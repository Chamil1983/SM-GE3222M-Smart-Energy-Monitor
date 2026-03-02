#pragma once

/**
 * SM-GE3222M V2.0 - LCD UI Manager (20x4 I2C)
 *
 * Non-invasive LCD UI implementation (services itself from a lightweight background task on ESP32)
 * to avoid interfering with existing Web UI/task scheduling.
 *
 * LCD: 20x4 HD44780 via I2C PCF8574 @ 0x27
 * Buttons: MODE=GPIO32, SET=GPIO33 (active-low)
 */

#include <Arduino.h>
#include "DataTypes.h"

class LCDUI20x4 {
public:
    static LCDUI20x4& getInstance();

    bool begin(uint8_t i2cAddr = 0x27);
    void loop();
    bool isInitialized() const { return _initialized; }

private:
    LCDUI20x4();
    ~LCDUI20x4() = default;
    LCDUI20x4(const LCDUI20x4&) = delete;
    LCDUI20x4& operator=(const LCDUI20x4&) = delete;

    enum class UiState : uint8_t {
        RUN = 0,
        MENU,
        CONFIRM
    };

    enum class RunPage : uint8_t {
        LIVE_SUMMARY = 0,
        PHASE_DETAIL,
        ENERGY_TOTALS,
        DEMAND_PEAKS,
        IO_STATUS,
        NETWORK_STATUS,
        SYSTEM_HEALTH,
        COUNT
    };

    enum class PowerView : uint8_t {
        KW = 0,
        KVA,
        KVAR
    };

    enum class CounterSelect : uint8_t {
        TODAY = 0,
        WEEK,
        MONTH
    };

    enum class ConfirmAction : uint8_t {
        NONE = 0,
        RESET_TRIP,
        RESET_TODAY,
        RESET_WEEK,
        RESET_MONTH
    };

    enum class MenuItem : uint8_t {
        POWER_VIEW = 0,
        DEMAND_WINDOW,
        PHASE_ROTATE,
        BACKLIGHT,
        RESET_TRIP,
        EXIT,
        COUNT
    };

    struct ButtonTracker {
        uint8_t pin = 0;
        bool rawState = HIGH;
        bool stableState = HIGH;
        uint32_t lastChangeMs = 0;
        uint32_t pressStartMs = 0;
        bool longReported = false;
    };

    struct ButtonEvents {
        bool modeShort = false;
        bool modeLong = false;
        bool setShort = false;
        bool setLong = false;
        bool comboHold3s = false;
    };

    // Core internals
    void pollButtons(ButtonEvents& ev);
    void handleEvents(const ButtonEvents& ev);
    void updateDerivedMetrics();
    void maybeRender(bool force = false);
    void renderCurrentScreen(bool force = false);

    // Rendering helpers
    void renderRunPage();
    void renderMenuPage();
    void renderConfirmPage();
    void writeLine(uint8_t row, const String& text, bool force = false);
    String fit20(const String& s) const;
    String fmtHeader(const char* title);
    String fmtFloat(float v, uint8_t prec, uint8_t width = 0) const;
    String fmtIpOrNA(const String& s) const;
    const char* errorCodeToShort(ErrorCode e) const;

    // Page renderers
    void renderPageLiveSummary();
    void renderPagePhaseDetail();
    void renderPageEnergyTotals();
    void renderPageDemandPeaks();
    void renderPageIOStatus();
    void renderPageNetwork();
    void renderPageSystem();

    // Actions / UI state
    void nextRunPage();
    void toggleRunSetAction();
    void runSetLongAction();
    void enterMenu();
    void exitMenu(bool showMsg = true);
    void menuNextItem();
    void menuSetShort();
    void menuSetLong();
    void openConfirm(ConfirmAction action);
    void cancelConfirm();
    void applyConfirmAction();
    void showToast(const String& msg, uint32_t durationMs = 1000);
    bool isConfirmExpired() const;
    bool isSettingsExpired() const;

    // Helpers/data calculation
    void integrateEnergyCounters(const MeterData& md, uint32_t nowMs);
    void updateDemandHistory(float pKw, uint32_t nowMs);
    float computeDemandAvgKw(uint8_t windowMinutes) const;
    void getSelectedPhase(const MeterData& md, PhaseData& out, char& phaseLabel);
    String powerViewLabel() const;

    // State
    bool _initialized;
    bool _lcdPresent;
    uint8_t _lcdAddr;

    // Opaque pointer to avoid exposing LCD lib in header
    class Impl;
    Impl* _impl;

    UiState _state;
    RunPage _page;
    uint32_t _lastUserInputMs;
    uint32_t _lastRenderMs;
    uint32_t _lastPhaseRotateMs;
    uint32_t _lastLoopSliceMs;
    uint32_t _lastMetricsUpdateMs;

    // Buttons
    ButtonTracker _btnMode;
    ButtonTracker _btnSet;
    bool _comboLatched;
    uint32_t _comboStartMs;

    // Line cache
    String _rowCache[4];

    // UI options
    PowerView _powerView;
    CounterSelect _counterSelect;
    uint8_t _phaseIndex;              // 0=A,1=B,2=C
    bool _phaseFreeze;
    uint8_t _demandWindowMin;         // 5 / 15 / 30
    bool _backlightOn;
    bool _netAltView;
    bool _ioAltView;

    // Confirm state
    ConfirmAction _confirmAction;
    UiState _confirmPrevState;
    RunPage _confirmPrevPage;
    uint32_t _confirmOpenedMs;
    bool _confirmSetHoldLatched;

    // Menu state
    uint8_t _menuIndex;
    uint32_t _savedMsgUntilMs;

    // Toast/non-fatal transient UI message
    String _toast;
    uint32_t _toastUntilMs;

    // Counters (software UI counters; safe/non-invasive)
    double _tripKWh;
    double _todayKWh;
    double _weekKWh;
    double _monthKWh;
    uint32_t _lastEnergyIntegrateMs;

    // Demand and peaks
    static const uint16_t DEMAND_SAMPLE_CAP = 360; // 5s samples => 30 minutes
    float _demandSamples[DEMAND_SAMPLE_CAP];
    uint16_t _demandHead;
    uint16_t _demandCount;
    uint32_t _lastDemandSampleMs;
    float _peakPkwToday;
    float _peakImaxToday;
};

