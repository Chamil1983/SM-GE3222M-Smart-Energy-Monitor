#include "LCDUI20x4.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

#include "PinMap.h"
#include "Version.h"
#include "Logger.h"
#include "I2CBus.h"
#include "GPIOManager.h"
#include "EnergyMeter.h"
#include "EnergyAccumulator.h"
#include "SystemMonitor.h"
#include "DataLogger.h"
#include "TCPDataServer.h"
#include "SMNetworkManager.h"

namespace {

template<int N> struct LcdInitPriority : LcdInitPriority<N-1> {};
template<> struct LcdInitPriority<0> {};

template<typename TLcd>
auto lcdInitCompat(TLcd& lcd, LcdInitPriority<2>) -> decltype(lcd.init(), void()) {
    lcd.init();
}

template<typename TLcd>
auto lcdInitCompat(TLcd& lcd, LcdInitPriority<1>) -> decltype(lcd.begin(), void()) {
    lcd.begin();
}

template<typename TLcd>
auto lcdInitCompat(TLcd& lcd, LcdInitPriority<0>) -> decltype(lcd.begin(20, 4), void()) {
    lcd.begin(20, 4);
}

} // namespace

// Opaque implementation holder (keeps LiquidCrystal_I2C type out of header)
class LCDUI20x4::Impl {
public:
    explicit Impl(uint8_t addr) : lcd(addr, 20, 4) {}
    LiquidCrystal_I2C lcd;
};

static constexpr uint32_t LCD_BTN_DEBOUNCE_MS      = 40;
static constexpr uint32_t LCD_BTN_LONG_MS          = 1200;
static constexpr uint32_t LCD_COMBO_HOLD_MS        = 3000;
static constexpr uint32_t LCD_SETTINGS_TIMEOUT_MS  = 60000;
static constexpr uint32_t LCD_CONFIRM_TIMEOUT_MS   = 15000;
static constexpr uint32_t LCD_PHASE_ROTATE_MS      = 2000;
static constexpr uint32_t LCD_DEMAND_SAMPLE_MS     = 5000;

LCDUI20x4& LCDUI20x4::getInstance() {
    static LCDUI20x4 instance;
    return instance;
}

LCDUI20x4::LCDUI20x4()
    : _initialized(false)
    , _lcdPresent(false)
    , _lcdAddr(0x27)
    , _impl(nullptr)
    , _state(UiState::RUN)
    , _page(RunPage::LIVE_SUMMARY)
    , _lastUserInputMs(0)
    , _lastRenderMs(0)
    , _lastPhaseRotateMs(0)
    , _comboLatched(false)
    , _comboStartMs(0)
    , _powerView(PowerView::KW)
    , _counterSelect(CounterSelect::TODAY)
    , _phaseIndex(0)
    , _phaseFreeze(false)
    , _demandWindowMin(15)
    , _backlightOn(true)
    , _netAltView(false)
    , _ioAltView(false)
    , _confirmAction(ConfirmAction::NONE)
    , _confirmPrevState(UiState::RUN)
    , _confirmPrevPage(RunPage::LIVE_SUMMARY)
    , _confirmOpenedMs(0)
    , _confirmSetHoldLatched(false)
    , _menuIndex(0)
    , _savedMsgUntilMs(0)
    , _toast("")
    , _toastUntilMs(0)
    , _tripKWh(0.0)
    , _todayKWh(0.0)
    , _weekKWh(0.0)
    , _monthKWh(0.0)
    , _lastEnergyIntegrateMs(0)
    , _demandHead(0)
    , _demandCount(0)
    , _lastDemandSampleMs(0)
    , _peakPkwToday(0.0f)
    , _peakImaxToday(0.0f)
{
    _btnMode.pin = PIN_BUTTON_MODE;
    _btnSet.pin  = PIN_BUTTON_SET;
    memset(_demandSamples, 0, sizeof(_demandSamples));
}

bool LCDUI20x4::begin(uint8_t i2cAddr) {
    if (_initialized) return true;

    _lcdAddr = i2cAddr;

    // Ensure I2C bus is initialized (idempotent)
    if (!I2CBus::getInstance().isInitialized()) {
        if (!I2CBus::getInstance().init()) {
            Logger::getInstance().warn("LCDUI: I2C bus init failed; LCD disabled");
            return false;
        }
    }

    // Probe LCD address to avoid hard-failing if LCD is disconnected
    Wire.beginTransmission(_lcdAddr);
    uint8_t err = Wire.endTransmission();
    if (err != 0) {
        Logger::getInstance().warn("LCDUI: LCD not found at 0x%02X (I2C err=%u); UI disabled", _lcdAddr, err);
        _lcdPresent = false;
        return false;
    }

    _impl = new Impl(_lcdAddr);
    if (!_impl) {
        Logger::getInstance().warn("LCDUI: Failed to allocate LCD driver; UI disabled");
        return false;
    }

    lcdInitCompat(_impl->lcd, LcdInitPriority<2>{});
    _impl->lcd.backlight();
    _impl->lcd.clear();
    _lcdPresent = true;

    for (int i = 0; i < 4; ++i) _rowCache[i] = "";

    // Splash (brief, non-invasive)
    writeLine(0, "GE3222M BOOT", true);
    writeLine(1, String("FW:") + FW_VERSION, true);
    writeLine(2, "LCD UI 20x4 Ready", true);
    writeLine(3, "WEB UI SAFE MODE", true);
    delay(600);

    _state = UiState::RUN;
    _page = RunPage::LIVE_SUMMARY;
    _lastUserInputMs = millis();
    _lastRenderMs = 0;
    _lastPhaseRotateMs = millis();

    _initialized = true;
    Logger::getInstance().info("LCDUI: Initialized (20x4 I2C @0x%02X, MODE=GPIO%d, SET=GPIO%d)",
                               _lcdAddr, PIN_BUTTON_MODE, PIN_BUTTON_SET);

    maybeRender(true);
    return true;
}

void LCDUI20x4::loop() {
    if (!_initialized || !_lcdPresent || _impl == nullptr) return;

    ButtonEvents ev;
    pollButtons(ev);
    handleEvents(ev);

    // Auto-timeouts
    if (_state == UiState::MENU && isSettingsExpired()) {
        _state = UiState::RUN;
        _page = RunPage::LIVE_SUMMARY;
        showToast("Settings timeout", 1200);
    }
    if (_state == UiState::CONFIRM && isConfirmExpired()) {
        cancelConfirm();
        showToast("Cancelled", 900);
    }

    updateDerivedMetrics();
    maybeRender(false);
}

void LCDUI20x4::pollButtons(ButtonEvents& ev) {
    uint32_t now = millis();

    auto updateOne = [&](ButtonTracker& b, bool& shortEvt, bool& longEvt) {
        bool raw = digitalRead(b.pin);
        if (raw != b.rawState) {
            b.rawState = raw;
            b.lastChangeMs = now;
        }

        if ((now - b.lastChangeMs) >= LCD_BTN_DEBOUNCE_MS && b.stableState != b.rawState) {
            b.stableState = b.rawState;
            if (b.stableState == LOW) {
                b.pressStartMs = now;
                b.longReported = false;
            } else {
                // Release -> short press if long wasn't reported and press duration valid
                uint32_t held = (b.pressStartMs > 0) ? (now - b.pressStartMs) : 0;
                if (!b.longReported && held >= LCD_BTN_DEBOUNCE_MS && held < LCD_BTN_LONG_MS) {
                    shortEvt = true;
                }
                b.pressStartMs = 0;
                b.longReported = false;
            }
        }

        if (b.stableState == LOW && !b.longReported && b.pressStartMs > 0) {
            uint32_t held = now - b.pressStartMs;
            if (held >= LCD_BTN_LONG_MS) {
                longEvt = true;
                b.longReported = true;
            }
        }
    };

    updateOne(_btnMode, ev.modeShort, ev.modeLong);
    updateOne(_btnSet, ev.setShort, ev.setLong);

    // Combo MODE+SET hold 3s (one-shot)
    bool modePressed = (_btnMode.stableState == LOW);
    bool setPressed  = (_btnSet.stableState == LOW);
    if (modePressed && setPressed) {
        if (_comboStartMs == 0) _comboStartMs = now;
        if (!_comboLatched && (now - _comboStartMs) >= LCD_COMBO_HOLD_MS) {
            ev.comboHold3s = true;
            _comboLatched = true;
            // suppress individual long events for this cycle
            ev.modeLong = false;
            ev.setLong = false;
        }
    } else {
        _comboStartMs = 0;
        _comboLatched = false;
    }

    // In confirm screen, SET must be held for 3s to confirm (one-shot)
    if (_state == UiState::CONFIRM) {
        if (setPressed && _btnSet.pressStartMs > 0 && !_confirmSetHoldLatched) {
            if ((now - _btnSet.pressStartMs) >= LCD_COMBO_HOLD_MS) {
                _confirmSetHoldLatched = true;
                applyConfirmAction();
                showToast("Done", 900);
            }
        }
        if (!setPressed) {
            _confirmSetHoldLatched = false;
        }
    }
}

void LCDUI20x4::handleEvents(const ButtonEvents& ev) {
    if (ev.modeShort || ev.modeLong || ev.setShort || ev.setLong || ev.comboHold3s) {
        _lastUserInputMs = millis();
    }

    // Quick action from RUN pages
    if (_state == UiState::RUN && ev.comboHold3s) {
        openConfirm(ConfirmAction::RESET_TRIP);
        return;
    }

    // Confirm screen handling
    if (_state == UiState::CONFIRM) {
        if (ev.modeShort || ev.modeLong) {
            cancelConfirm();
            showToast("Cancelled", 900);
        }
        return;
    }

    if (_state == UiState::RUN) {
        if (ev.modeLong) {
            enterMenu();
            return;
        }
        if (ev.modeShort) {
            nextRunPage();
        }
        if (ev.setShort) {
            toggleRunSetAction();
        }
        if (ev.setLong) {
            runSetLongAction();
        }
        return;
    }

    if (_state == UiState::MENU) {
        if (ev.modeLong) {
            exitMenu(true);
            return;
        }
        if (ev.modeShort) menuNextItem();
        if (ev.setShort)  menuSetShort();
        if (ev.setLong)   menuSetLong();
        return;
    }
}

void LCDUI20x4::updateDerivedMetrics() {
    uint32_t now = millis();

    // Keep system metrics fresh for LCD page 7
    SystemMonitor::getInstance().update();

    MeterData md = EnergyMeter::getInstance().getSnapshot();
    if (md.valid) {
        integrateEnergyCounters(md, now);

        float pKw = md.totalActivePower / 1000.0f;
        if (pKw < 0.0f) pKw = 0.0f;
        updateDemandHistory(pKw, now);

        if (pKw > _peakPkwToday) _peakPkwToday = pKw;
        float iMax = md.phaseA.currentRMS;
        if (md.phaseB.currentRMS > iMax) iMax = md.phaseB.currentRMS;
        if (md.phaseC.currentRMS > iMax) iMax = md.phaseC.currentRMS;
        if (iMax > _peakImaxToday) _peakImaxToday = iMax;
    }

    if (_state == UiState::RUN && _page == RunPage::PHASE_DETAIL && !_phaseFreeze) {
        if ((now - _lastPhaseRotateMs) >= LCD_PHASE_ROTATE_MS) {
            _phaseIndex = (uint8_t)((_phaseIndex + 1) % 3);
            _lastPhaseRotateMs = now;
            maybeRender(true);
        }
    }
}

void LCDUI20x4::maybeRender(bool force) {
    if (!_initialized || !_lcdPresent || _impl == nullptr) return;

    uint32_t now = millis();
    uint32_t intervalMs = 1000;

    if (_state == UiState::RUN) {
        switch (_page) {
            case RunPage::LIVE_SUMMARY:   intervalMs = 400; break;
            case RunPage::PHASE_DETAIL:   intervalMs = 500; break;
            case RunPage::ENERGY_TOTALS:  intervalMs = 1000; break;
            case RunPage::DEMAND_PEAKS:   intervalMs = 1000; break;
            case RunPage::IO_STATUS:      intervalMs = 1000; break;
            case RunPage::NETWORK_STATUS: intervalMs = 1500; break;
            case RunPage::SYSTEM_HEALTH:  intervalMs = 1000; break;
            default: intervalMs = 1000; break;
        }
    } else {
        intervalMs = 80; // event-driven feel, still capped
    }

    bool timed = (now - _lastRenderMs) >= intervalMs;
    bool toastActive = (_toastUntilMs > now);
    bool savedMsgActive = (_savedMsgUntilMs > now);

    if (force || timed || toastActive || savedMsgActive) {
        renderCurrentScreen(force);
        _lastRenderMs = now;
    }
}

void LCDUI20x4::renderCurrentScreen(bool force) {
    if (force) { for (int i = 0; i < 4; ++i) _rowCache[i] = ""; }
    if (_state == UiState::RUN)       renderRunPage();
    else if (_state == UiState::MENU)  renderMenuPage();
    else if (_state == UiState::CONFIRM) renderConfirmPage();

    // Optional toast overlays line 4 (bottom line) for readability, then normal rendering resumes.
    if (_toastUntilMs > millis() && _toast.length() > 0) {
        writeLine(3, _toast, true);
    }

    // Keep compiler happy if not using force in future changes
    (void)force;
}

void LCDUI20x4::renderRunPage() {
    switch (_page) {
        case RunPage::LIVE_SUMMARY:   renderPageLiveSummary(); break;
        case RunPage::PHASE_DETAIL:   renderPagePhaseDetail(); break;
        case RunPage::ENERGY_TOTALS:  renderPageEnergyTotals(); break;
        case RunPage::DEMAND_PEAKS:   renderPageDemandPeaks(); break;
        case RunPage::IO_STATUS:      renderPageIOStatus(); break;
        case RunPage::NETWORK_STATUS: renderPageNetwork(); break;
        case RunPage::SYSTEM_HEALTH:  renderPageSystem(); break;
        default: renderPageLiveSummary(); break;
    }
}

void LCDUI20x4::renderMenuPage() {
    writeLine(0, "SETTINGS > LCD UI");

    auto itemText = [&](uint8_t idx) -> String {
        switch ((MenuItem)idx) {
            case MenuItem::POWER_VIEW:
                return String("P View: ") + powerViewLabel();
            case MenuItem::DEMAND_WINDOW:
                return String("Demand: ") + String(_demandWindowMin) + "m";
            case MenuItem::PHASE_ROTATE:
                return String("Phase Rot: ") + (_phaseFreeze ? "Freeze" : "Auto");
            case MenuItem::BACKLIGHT:
                return String("Backlight: ") + (_backlightOn ? "ON" : "OFF");
            case MenuItem::RESET_TRIP:
                return "Reset Trip Energy";
            case MenuItem::EXIT:
                return "Back / Exit";
            default:
                return "";
        }
    };

    // Simple scrolling list over 3 visible rows
    uint8_t total = (uint8_t)MenuItem::COUNT;
    uint8_t start = 0;
    if (_menuIndex >= 2) start = _menuIndex - 2;
    if (start > total - 3) start = (total > 3) ? (total - 3) : 0;

    for (uint8_t row = 0; row < 3; ++row) {
        uint8_t idx = start + row;
        String s = (idx == _menuIndex ? ">" : " ");
        s += itemText(idx);
        writeLine(row + 1, s);
    }

    if (_savedMsgUntilMs > millis()) {
        writeLine(3, "Saved", true);
    }
}

void LCDUI20x4::renderConfirmPage() {
    String l1 = "Confirm Action";
    String l2 = "";

    switch (_confirmAction) {
        case ConfirmAction::RESET_TRIP:  l2 = "RESET TRIP ENERGY?"; break;
        case ConfirmAction::RESET_TODAY: l2 = "RESET TODAY kWh?"; break;
        case ConfirmAction::RESET_WEEK:  l2 = "RESET WEEK kWh?"; break;
        case ConfirmAction::RESET_MONTH: l2 = "RESET MONTH kWh?"; break;
        default: l2 = "No action"; break;
    }

    writeLine(0, l1);
    writeLine(1, l2);
    writeLine(2, "Hold SET 3s confirm");
    writeLine(3, "MODE = Cancel");
}

void LCDUI20x4::writeLine(uint8_t row, const String& text, bool force) {
    if (!_impl || row > 3) return;
    String out = fit20(text);
    if (!force && _rowCache[row] == out) return;
    _rowCache[row] = out;
    _impl->lcd.setCursor(0, row);
    _impl->lcd.print(out);
}

String LCDUI20x4::fit20(const String& s) const {
    String out = s;
    if (out.length() > 20) out = out.substring(0, 20);
    while (out.length() < 20) out += ' ';
    return out;
}

String LCDUI20x4::fmtHeader(const char* title) {
    SystemStatus st = SystemMonitor::getInstance().getSystemStatus();
    uint32_t up = st.uptime;
    uint8_t hh = (up / 3600U) % 24U;
    uint8_t mm = (up / 60U) % 60U;

    char buf[24];
    char alarm = (st.lastError != ErrorCode::NONE || st.errorCount > 0) ? '!' : ' ';
    snprintf(buf, sizeof(buf), "%s%c %02u:%02u", title, alarm, (unsigned)hh, (unsigned)mm);
    return String(buf);
}

String LCDUI20x4::fmtFloat(float v, uint8_t prec, uint8_t width) const {
    if (!isfinite(v)) return "--";
    String s((double)v, (unsigned int)prec);
    if (width > 0) {
        while (s.length() < width) s = String(" ") + s;
    }
    return s;
}

String LCDUI20x4::fmtIpOrNA(const String& s) const {
    if (s.length() == 0) return "N/A";
    return s;
}

const char* LCDUI20x4::errorCodeToShort(ErrorCode e) const {
    switch (e) {
        case ErrorCode::NONE: return "NONE";
        case ErrorCode::ATM_INIT_FAILED: return "METER";
        case ErrorCode::WIFI_CONNECT_FAILED: return "WIFI";
        case ErrorCode::NVS_INIT_FAILED: return "NVS";
        case ErrorCode::TCP_SERVER_ERROR: return "TCP";
        case ErrorCode::MODBUS_INIT_ERROR: return "MODBUS";
        default: return "ERR";
    }
}

void LCDUI20x4::renderPageLiveSummary() {
    MeterData md = EnergyMeter::getInstance().getSnapshot();
    writeLine(0, fmtHeader("GE3222M RUN"));

    if (!md.valid) {
        writeLine(1, "V:--.- I:--.--A");
        writeLine(2, "P:--.--kW PF:--");
        writeLine(3, "E:------ kWh F:--");
        return;
    }

    // Summary uses avg voltage across valid phases and max phase current for quick field readability.
    float vSum = 0.0f; uint8_t vCnt = 0;
    if (md.phaseA.voltageRMS > 1.0f) { vSum += md.phaseA.voltageRMS; vCnt++; }
    if (md.phaseB.voltageRMS > 1.0f) { vSum += md.phaseB.voltageRMS; vCnt++; }
    if (md.phaseC.voltageRMS > 1.0f) { vSum += md.phaseC.voltageRMS; vCnt++; }
    float vDisp = (vCnt > 0) ? (vSum / (float)vCnt) : md.phaseA.voltageRMS;
    float iDisp = md.phaseA.currentRMS;
    if (md.phaseB.currentRMS > iDisp) iDisp = md.phaseB.currentRMS;
    if (md.phaseC.currentRMS > iDisp) iDisp = md.phaseC.currentRMS;

    char line2[32];
    snprintf(line2, sizeof(line2), "V:%5.1f I:%5.2fA", vDisp, iDisp);
    writeLine(1, line2);

    float pVal = 0.0f;
    const char* unit = "kW";
    if (_powerView == PowerView::KW) {
        pVal = md.totalActivePower / 1000.0f;
        unit = "kW";
    } else if (_powerView == PowerView::KVA) {
        pVal = md.totalApparentPower / 1000.0f;
        unit = "kVA";
    } else {
        pVal = md.totalReactivePower / 1000.0f;
        unit = "kvar";
    }

    char line3[32];
    snprintf(line3, sizeof(line3), "P:%5.2f%-4s PF:%0.2f", pVal, unit, md.totalPowerFactor);
    writeLine(2, line3);

    char line4[40];
    snprintf(line4, sizeof(line4), "E:%7.2fkWh F:%4.1f", md.totalFwdActiveEnergy, md.frequency);
    writeLine(3, line4);
}

void LCDUI20x4::getSelectedPhase(const MeterData& md, PhaseData& out, char& phaseLabel) {
    uint8_t idx = _phaseIndex % 3;
    if (idx == 0) { out = md.phaseA; phaseLabel = '1'; }
    else if (idx == 1) { out = md.phaseB; phaseLabel = '2'; }
    else { out = md.phaseC; phaseLabel = '3'; }

    // If selected phase looks empty but A is valid, fallback to A (single-phase systems)
    bool looksEmpty = (out.voltageRMS < 1.0f && fabsf(out.currentRMS) < 0.001f && fabsf(out.activePower) < 0.01f);
    if (looksEmpty && (md.phaseA.voltageRMS > 1.0f || fabsf(md.phaseA.currentRMS) > 0.001f)) {
        out = md.phaseA;
        phaseLabel = '1';
    }
}

void LCDUI20x4::renderPagePhaseDetail() {
    MeterData md = EnergyMeter::getInstance().getSnapshot();
    if (!md.valid) {
        writeLine(0, "PHASE --");
        writeLine(1, "No meter data");
        writeLine(2, "Waiting...");
        writeLine(3, "");
        return;
    }

    PhaseData ph;
    char phLabel = '1';
    getSelectedPhase(md, ph, phLabel);

    char hdr[24];
    snprintf(hdr, sizeof(hdr), "PHASE L%c %s", phLabel, _phaseFreeze ? "(FIX)" : "(ROT)");
    writeLine(0, hdr);

    char l2[32];
    snprintf(l2, sizeof(l2), "V:%5.1f I:%5.2fA", ph.voltageRMS, ph.currentRMS);
    writeLine(1, l2);

    char l3[40];
    snprintf(l3, sizeof(l3), "P:%5.2fkW PF:%0.2f", ph.activePower / 1000.0f, ph.powerFactor);
    writeLine(2, l3);

    char l4[40];
    snprintf(l4, sizeof(l4), "THDv:%3.1f THDi:%3.1f", ph.voltageTHDN, ph.currentTHDN);
    writeLine(3, l4);
}

void LCDUI20x4::renderPageEnergyTotals() {
    MeterData md = EnergyMeter::getInstance().getSnapshot();
    writeLine(0, "ENERGY TOTALS");

    char l2[40];
    snprintf(l2, sizeof(l2), "Import:%8.2f", md.totalFwdActiveEnergy);
    writeLine(1, String(l2) + " kWh");

    char l3[40];
    snprintf(l3, sizeof(l3), "Export:%8.2f", md.totalRevActiveEnergy);
    writeLine(2, String(l3) + " kWh");

    const char* label = "Today";
    double val = _todayKWh;
    if (_counterSelect == CounterSelect::WEEK)  { label = "Week";  val = _weekKWh; }
    if (_counterSelect == CounterSelect::MONTH) { label = "Month"; val = _monthKWh; }

    char l4[40];
    snprintf(l4, sizeof(l4), "%s:%8.2f kWh", label, (float)val);
    writeLine(3, l4);
}

void LCDUI20x4::renderPageDemandPeaks() {
    writeLine(0, "DEMAND / PEAKS");

    float pavg = computeDemandAvgKw(_demandWindowMin);
    char l2[40];
    snprintf(l2, sizeof(l2), "Pavg(%2um): %5.2fkW", (unsigned)_demandWindowMin, pavg);
    writeLine(1, l2);

    char l3[40];
    snprintf(l3, sizeof(l3), "Pmax Today:%5.2fkW", _peakPkwToday);
    writeLine(2, l3);

    char l4[40];
    snprintf(l4, sizeof(l4), "Imax Today:%5.2fA", _peakImaxToday);
    writeLine(3, l4);
}

void LCDUI20x4::renderPageIOStatus() {
    writeLine(0, "I/O STATUS");

    // Relay outputs are reserved for future GPIO control integration; keep view-only placeholders.
    if (!_ioAltView) {
        writeLine(1, "DO1:-- DO2:--");

        bool di1 = GPIOManager::getInstance().readPortB(0);
        bool di2 = GPIOManager::getInstance().readPortB(1);
        bool di3 = GPIOManager::getInstance().readPortB(2);
        bool di4 = GPIOManager::getInstance().readPortB(3);
        char l3[40];
        snprintf(l3, sizeof(l3), "DI:%u=%u %u=%u %u=%u %u=%u",
                 1, di1 ? 1 : 0,
                 2, di2 ? 1 : 0,
                 3, di3 ? 1 : 0,
                 4, di4 ? 1 : 0);
        writeLine(2, l3);
    } else {
        bool in1 = GPIOManager::getInstance().readPortB(4);
        bool in2 = GPIOManager::getInstance().readPortB(5);
        bool in3 = GPIOManager::getInstance().readPortB(6);
        bool in4 = GPIOManager::getInstance().readPortB(7);
        writeLine(1, "ALT IN (PortB 4-7)");
        char l3[40];
        snprintf(l3, sizeof(l3), "P4:%u P5:%u P6:%u P7:%u", in1?1:0, in2?1:0, in3?1:0, in4?1:0);
        writeLine(2, l3);
    }

    SystemStatus st = SystemMonitor::getInstance().getSystemStatus();
    String alarm = (st.lastError == ErrorCode::NONE && st.errorCount == 0)
        ? "Alarm: NONE"
        : (String("Alarm: ") + errorCodeToShort(st.lastError));
    writeLine(3, alarm);
}

void LCDUI20x4::renderPageNetwork() {
    writeLine(0, "NETWORK");

    bool ap = networkManager.isAPMode();
    bool sta = networkManager.isSTAMode();
    bool conn = networkManager.isConnected();
    String ip = fmtIpOrNA(networkManager.getIPAddress());

    if (!_netAltView) {
        String line2;
        if (ap)       line2 = String("AP :") + ip;
        else if (sta) line2 = String("STA:") + ip;
        else          line2 = "WiFi:OFF";
        if (!conn) line2 += " DOWN";
        writeLine(1, line2);

        char l3[40];
        snprintf(l3, sizeof(l3), "WiFi:%s AP:%s", conn ? "ON" : "OFF", ap ? "ON" : "OFF");
        writeLine(2, l3);

        bool tcpRun = TCPDataServer::getInstance().isRunning();
        uint16_t tcpPort = TCPDataServer::getInstance().getPort();
        char l4[40];
        snprintf(l4, sizeof(l4), "WEB:80 TCP:%u %s", (unsigned)tcpPort, tcpRun ? "OK" : "OFF");
        writeLine(3, l4);
    } else {
        writeLine(1, String("SSID:") + networkManager.getSSID());
        writeLine(2, String("M:") + networkManager.getMacAddress());
        char l4[40];
        snprintf(l4, sizeof(l4), "TCP Cli:%u", (unsigned)TCPDataServer::getInstance().getClientCount());
        writeLine(3, l4);
    }
}

void LCDUI20x4::renderPageSystem() {
    SystemStatus st = SystemMonitor::getInstance().getSystemStatus();
    writeLine(0, "SYSTEM");

    uint32_t up = st.uptime;
    uint32_t hh = up / 3600U;
    uint32_t mm = (up / 60U) % 60U;
    uint32_t ss = up % 60U;
    char l2[32];
    snprintf(l2, sizeof(l2), "Uptime:%02lu:%02lu:%02lu", (unsigned long)hh, (unsigned long)mm, (unsigned long)ss);
    writeLine(1, l2);

    char l3[40];
    snprintf(l3, sizeof(l3), "Heap:%luk Min:%luk",
             (unsigned long)(st.freeHeap / 1024UL),
             (unsigned long)(st.minFreeHeap / 1024UL));
    writeLine(2, l3);

    size_t logCount = DataLogger::getInstance().getBufferCount();
    size_t logCap   = DataLogger::getInstance().getBufferSize();
    char l4[40];
    snprintf(l4, sizeof(l4), "WDT:OK Log:%u/%u", (unsigned)logCount, (unsigned)logCap);
    writeLine(3, l4);
}

void LCDUI20x4::nextRunPage() {
    _page = (RunPage)(((uint8_t)_page + 1U) % (uint8_t)RunPage::COUNT);
    maybeRender(true);
}

void LCDUI20x4::toggleRunSetAction() {
    switch (_page) {
        case RunPage::LIVE_SUMMARY:
            _powerView = (PowerView)(((uint8_t)_powerView + 1U) % 3U);
            break;
        case RunPage::PHASE_DETAIL:
            _phaseFreeze = !_phaseFreeze;
            showToast(_phaseFreeze ? "Phase freeze ON" : "Phase freeze OFF", 900);
            break;
        case RunPage::ENERGY_TOTALS:
            _counterSelect = (CounterSelect)(((uint8_t)_counterSelect + 1U) % 3U);
            break;
        case RunPage::DEMAND_PEAKS:
            if (_demandWindowMin == 5) _demandWindowMin = 15;
            else if (_demandWindowMin == 15) _demandWindowMin = 30;
            else _demandWindowMin = 5;
            break;
        case RunPage::IO_STATUS:
            _ioAltView = !_ioAltView;
            break;
        case RunPage::NETWORK_STATUS:
            _netAltView = !_netAltView;
            break;
        case RunPage::SYSTEM_HEALTH:
            SystemMonitor::getInstance().setLastError(ErrorCode::NONE);
            SystemMonitor::getInstance().resetErrorCount();
            showToast("Errors cleared", 900);
            break;
        default:
            break;
    }
    maybeRender(true);
}

void LCDUI20x4::runSetLongAction() {
    if (_page == RunPage::ENERGY_TOTALS) {
        if (_counterSelect == CounterSelect::TODAY) openConfirm(ConfirmAction::RESET_TODAY);
        else if (_counterSelect == CounterSelect::WEEK) openConfirm(ConfirmAction::RESET_WEEK);
        else openConfirm(ConfirmAction::RESET_MONTH);
    }
}

void LCDUI20x4::enterMenu() {
    _state = UiState::MENU;
    _menuIndex = 0;
    _savedMsgUntilMs = 0;
    maybeRender(true);
}

void LCDUI20x4::exitMenu(bool showMsg) {
    _state = UiState::RUN;
    _page = RunPage::LIVE_SUMMARY;
    if (showMsg) showToast("Exit settings", 800);
    maybeRender(true);
}

void LCDUI20x4::menuNextItem() {
    _menuIndex = (uint8_t)((_menuIndex + 1U) % (uint8_t)MenuItem::COUNT);
    maybeRender(true);
}

void LCDUI20x4::menuSetShort() {
    switch ((MenuItem)_menuIndex) {
        case MenuItem::POWER_VIEW:
            _powerView = (PowerView)(((uint8_t)_powerView + 1U) % 3U);
            break;
        case MenuItem::DEMAND_WINDOW:
            if (_demandWindowMin == 5) _demandWindowMin = 15;
            else if (_demandWindowMin == 15) _demandWindowMin = 30;
            else _demandWindowMin = 5;
            break;
        case MenuItem::PHASE_ROTATE:
            _phaseFreeze = !_phaseFreeze;
            break;
        case MenuItem::BACKLIGHT:
            _backlightOn = !_backlightOn;
            if (_impl) {
                if (_backlightOn) _impl->lcd.backlight();
                else _impl->lcd.noBacklight();
            }
            break;
        case MenuItem::RESET_TRIP:
            openConfirm(ConfirmAction::RESET_TRIP);
            return;
        case MenuItem::EXIT:
            exitMenu(true);
            return;
        default:
            break;
    }
    maybeRender(true);
}

void LCDUI20x4::menuSetLong() {
    _savedMsgUntilMs = millis() + 1000;
    showToast("Saved", 900);
    maybeRender(true);
}

void LCDUI20x4::openConfirm(ConfirmAction action) {
    _confirmPrevState = _state;
    _confirmPrevPage = _page;
    _confirmAction = action;
    _confirmOpenedMs = millis();
    _confirmSetHoldLatched = false;
    _state = UiState::CONFIRM;
    maybeRender(true);
}

void LCDUI20x4::cancelConfirm() {
    _state = _confirmPrevState;
    _page = _confirmPrevPage;
    _confirmAction = ConfirmAction::NONE;
    _confirmOpenedMs = 0;
    _confirmSetHoldLatched = false;
    maybeRender(true);
}

void LCDUI20x4::applyConfirmAction() {
    switch (_confirmAction) {
        case ConfirmAction::RESET_TRIP:  _tripKWh = 0.0; break;
        case ConfirmAction::RESET_TODAY: _todayKWh = 0.0; break;
        case ConfirmAction::RESET_WEEK:  _weekKWh = 0.0; break;
        case ConfirmAction::RESET_MONTH: _monthKWh = 0.0; break;
        default: break;
    }

    _state = _confirmPrevState;
    _page = _confirmPrevPage;
    _confirmAction = ConfirmAction::NONE;
    _confirmOpenedMs = 0;
    _confirmSetHoldLatched = false;
    maybeRender(true);
}

void LCDUI20x4::showToast(const String& msg, uint32_t durationMs) {
    _toast = msg;
    _toastUntilMs = millis() + durationMs;
}

bool LCDUI20x4::isConfirmExpired() const {
    return (_state == UiState::CONFIRM) && (_confirmOpenedMs > 0) && ((millis() - _confirmOpenedMs) >= LCD_CONFIRM_TIMEOUT_MS);
}

bool LCDUI20x4::isSettingsExpired() const {
    return (_state == UiState::MENU) && ((millis() - _lastUserInputMs) >= LCD_SETTINGS_TIMEOUT_MS);
}

void LCDUI20x4::integrateEnergyCounters(const MeterData& md, uint32_t nowMs) {
    if (_lastEnergyIntegrateMs == 0) {
        _lastEnergyIntegrateMs = nowMs;
        return;
    }

    uint32_t dtMs = nowMs - _lastEnergyIntegrateMs;
    if (dtMs == 0 || dtMs > 10000) { // skip long gaps
        _lastEnergyIntegrateMs = nowMs;
        return;
    }

    float pW = md.totalActivePower;
    if (pW < 0.0f) pW = 0.0f; // import only
    double dKWh = ((double)pW * (double)dtMs) / 3600000000.0; // W*ms -> kWh

    _tripKWh  += dKWh;
    _todayKWh += dKWh;
    _weekKWh  += dKWh;
    _monthKWh += dKWh;

    _lastEnergyIntegrateMs = nowMs;
}

void LCDUI20x4::updateDemandHistory(float pKw, uint32_t nowMs) {
    if (_lastDemandSampleMs != 0 && (nowMs - _lastDemandSampleMs) < LCD_DEMAND_SAMPLE_MS) return;

    _demandSamples[_demandHead] = pKw;
    _demandHead = (uint16_t)((_demandHead + 1U) % DEMAND_SAMPLE_CAP);
    if (_demandCount < DEMAND_SAMPLE_CAP) _demandCount++;
    _lastDemandSampleMs = nowMs;
}

float LCDUI20x4::computeDemandAvgKw(uint8_t windowMinutes) const {
    if (_demandCount == 0) return 0.0f;

    uint16_t samplesNeeded = 0;
    if (windowMinutes == 5) samplesNeeded = 60;
    else if (windowMinutes == 15) samplesNeeded = 180;
    else samplesNeeded = 360;

    if (samplesNeeded > _demandCount) samplesNeeded = _demandCount;
    if (samplesNeeded == 0) return 0.0f;

    float sum = 0.0f;
    for (uint16_t i = 0; i < samplesNeeded; ++i) {
        int idx = (int)_demandHead - 1 - (int)i;
        while (idx < 0) idx += DEMAND_SAMPLE_CAP;
        sum += _demandSamples[idx];
    }
    return sum / (float)samplesNeeded;
}

String LCDUI20x4::powerViewLabel() const {
    switch (_powerView) {
        case PowerView::KW:   return "kW";
        case PowerView::KVA:  return "kVA";
        case PowerView::KVAR: return "kvar";
        default: return "kW";
    }
}

