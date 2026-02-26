#include "WebUIManager.h"

#include <ArduinoJson.h>
#include <WiFi.h>
#include "SPIFFSManager.h"
#include "SMNetworkManager.h"
#include "Logger.h"
#include "EnergyMeter.h"
#include "EnergyAccumulator.h"
#include "SystemMonitor.h"
#include "ProtocolV2.h"

static const char PAGE_CONFIG_TEMPLATE[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>SM-GE3222M Setup</title>
<style>
body{font-family:Arial,sans-serif;max-width:480px;margin:24px auto;padding:16px;background:#f5f5f5}
.box{background:#fff;padding:20px;border-radius:8px;box-shadow:0 2px 6px rgba(0,0,0,.1)}
input,select,button{width:100%;padding:10px;margin:6px 0 12px;box-sizing:border-box}
.hidden{display:none} .muted{color:#666;font-size:12px}
a{color:#0b66c3}
</style>
<script>
function toggleStatic(){document.getElementById('sf').className=document.getElementById('mode').value==='static'?'':'hidden';}
function fill(){
 fetch('/api/config').then(r=>r.json()).then(j=>{
   const w=(j&&j.wifi)?j.wifi:{};
   if(w.ssid) ssid.value=w.ssid;
   if(w.useDHCP===false){ mode.value='static'; toggleStatic(); }
   if(w.ip) ip.value=w.ip; if(w.gw) gw.value=w.gw; if(w.sn) sn.value=w.sn;
   if(w.dns1) dns1.value=w.dns1; if(w.dns2) dns2.value=w.dns2;
 }).catch(()=>{});
}
window.addEventListener('DOMContentLoaded', fill);
</script>
</head><body>
<div class="box">
<h2>📶 WiFi Configuration</h2>
<p>Setup AP: <b>%AP_SSID%</b> (%AP_IP%)</p>
<form action="/save" method="POST">
<label>Network SSID</label><input id="ssid" type="text" name="ssid" placeholder="WiFi Name" required>
<label>Password</label><input type="password" name="pass" placeholder="WiFi Password">
<label>IP Mode</label><select id="mode" name="mode" onchange="toggleStatic()"><option value="dhcp">DHCP — Automatic</option><option value="static">Static IP — Manual</option></select>
<div id="sf" class="hidden">
<label>Static IP</label><input id="ip" type="text" name="ip" placeholder="192.168.1.100">
<label>Gateway</label><input id="gw" type="text" name="gw" placeholder="192.168.1.1">
<label>Subnet Mask</label><input id="sn" type="text" name="sn" placeholder="255.255.255.0">
<label>Primary DNS</label><input id="dns1" type="text" name="dns1" placeholder="8.8.8.8">
<label>Secondary DNS</label><input id="dns2" type="text" name="dns2" placeholder="8.8.4.4">
</div>
<button type="submit">✔ Save & Connect</button>
</form>
<p class="muted">After successful STA connection, open <a href="/index.html">main meter webpage</a>.</p>
</div></body></html>)rawliteral";

WebUIManager& WebUIManager::getInstance() { static WebUIManager i; return i; }
WebUIManager::WebUIManager() : _server(80), _running(false), _port(80) {}

bool WebUIManager::begin(uint16_t port) {
    _port = port;
    setupRoutes();
    _server.begin();
    _running = true;
    Logger::getInstance().info("WebUI: HTTP server started on port %u", (unsigned)port);
    return true;
}

void WebUIManager::loop() {
    if (_running) _server.handleClient();
}

void WebUIManager::setupRoutes() {
    _server.on("/", HTTP_GET, [this]() { handleRoot(); });
    _server.on("/index.html", HTTP_GET, [this]() { handleIndex(); });
    _server.on("/style.css", HTTP_GET, [this]() { handleStaticFile("/style.css", "text/css"); });
    _server.on("/dashboard.js", HTTP_GET, [this]() { handleStaticFile("/dashboard.js", "application/javascript"); });

    _server.on("/api/meter", HTTP_GET, [this]() { handleApiMeter(); });
    _server.on("/api/status", HTTP_GET, [this]() { handleApiStatus(); });
    _server.on("/api/config", HTTP_GET, [this]() { handleApiConfigGet(); });
    _server.on("/api/config", HTTP_POST, [this]() { handleApiConfigPost(); });
    _server.on("/save", HTTP_POST, [this]() { handleSaveForm(); });

    // Captive portal probes
    _server.on("/generate_204", HTTP_GET, [this]() { handleCaptiveRedirect(); });
    _server.on("/fwlink", HTTP_GET, [this]() { handleCaptiveRedirect(); });
    _server.on("/connecttest.txt", HTTP_GET, [this]() { handleCaptiveRedirect(); });
    _server.on("/hotspot-detect.html", HTTP_GET, [this]() { handleCaptiveRedirect(); });
    _server.on("/library/test/success.html", HTTP_GET, [this]() { handleCaptiveRedirect(); });
    _server.on("/success.txt", HTTP_GET, [this]() { handleCaptiveRedirect(); });

    _server.onNotFound([this]() { handleNotFound(); });
}

void WebUIManager::handleRoot() {
    if (networkManager.isAPMode()) {
        String p = buildWiFiSetupPage();
        _server.send(200, "text/html", p);
        return;
    }
    handleIndex();
}

void WebUIManager::handleIndex() {
    if (SPIFFS.exists("/index.html")) {
        handleStaticFile("/index.html", "text/html");
        return;
    }
    _server.send(200, "text/html", "<html><body><h3>Web UI files not found in SPIFFS.</h3><p>Upload data folder to SPIFFS.</p></body></html>");
}

void WebUIManager::handleStaticFile(const String& path, const String& contentType) {
    if (!SPIFFS.exists(path)) {
        _server.send(404, "text/plain", "File not found");
        return;
    }
    File f = SPIFFS.open(path, "r");
    _server.streamFile(f, contentType);
    f.close();
}

void WebUIManager::handleApiMeter() {
    // use latest snapshot; if stale, try refresh
    EnergyMeter::getInstance().update();
    MeterData m = EnergyMeter::getInstance().getSnapshot();
    EnergyData e = EnergyAccumulator::getInstance().getAccumulatedEnergy();
    SystemStatus s = SystemMonitor::getInstance().getSystemStatus();

    DynamicJsonDocument doc(3072);
    // Flat keys expected by dashboard.js
    doc["UrmsA"] = m.phaseA.voltageRMS;
    doc["UrmsB"] = m.phaseB.voltageRMS;
    doc["UrmsC"] = m.phaseC.voltageRMS;
    doc["IrmsA"] = m.phaseA.currentRMS;
    doc["IrmsB"] = m.phaseB.currentRMS;
    doc["IrmsC"] = m.phaseC.currentRMS;
    doc["PmeanA"] = m.phaseA.activePower / 1000.0f;
    doc["PmeanB"] = m.phaseB.activePower / 1000.0f;
    doc["PmeanC"] = m.phaseC.activePower / 1000.0f;
    doc["PmeanT"] = m.totalActivePower / 1000.0f;
    doc["QmeanA"] = m.phaseA.reactivePower / 1000.0f;
    doc["QmeanB"] = m.phaseB.reactivePower / 1000.0f;
    doc["QmeanC"] = m.phaseC.reactivePower / 1000.0f;
    doc["QmeanT"] = m.totalReactivePower / 1000.0f;
    doc["SmeanA"] = m.phaseA.apparentPower / 1000.0f;
    doc["SmeanB"] = m.phaseB.apparentPower / 1000.0f;
    doc["SmeanC"] = m.phaseC.apparentPower / 1000.0f;
    doc["SAmeanT"] = m.totalApparentPower / 1000.0f;
    doc["PFmeanA"] = m.phaseA.powerFactor;
    doc["PFmeanB"] = m.phaseB.powerFactor;
    doc["PFmeanC"] = m.phaseC.powerFactor;
    doc["PFmeanT"] = m.totalPowerFactor;
    doc["Freq"] = m.frequency;
    doc["Temp"] = (m.boardTemperature != 0.0f ? m.boardTemperature : m.ambientTemperature);

    doc["APenergyT"] = e.total.activeEnergyImport;
    doc["ANenergyT"] = e.total.activeEnergyExport;
    doc["RPenergyT"] = e.total.reactiveEnergyImport;
    doc["RNenergyT"] = e.total.reactiveEnergyExport;

    doc["uptime"] = s.uptime;
    doc["freeHeap"] = s.freeHeap;
    doc["ipaddress"] = networkManager.getIPAddress();
    doc["rssi"] = (networkManager.isSTAMode() ? WiFi.RSSI() : 0);
    doc["wifiMode"] = networkManager.isAPMode() ? "AP" : (networkManager.isSTAMode() ? "STA" : "NONE");
    doc["ssid"] = networkManager.getSSID();

    String out; serializeJson(doc, out);
    sendJson(200, out);
}

void WebUIManager::handleApiStatus() {
    DynamicJsonDocument doc(1024);
    SystemStatus s = SystemMonitor::getInstance().getSystemStatus();
    ProtocolV2::getInstance().systemStatusToJson(s, doc);
    doc["ip"] = networkManager.getIPAddress();
    doc["ssid"] = networkManager.getSSID();
    doc["mac"] = networkManager.getMacAddress();
    String out; serializeJson(doc, out);
    sendJson(200, out);
}

void WebUIManager::handleApiConfigGet() {
    DynamicJsonDocument doc(4096);
    ProtocolV2::getInstance().configToJson(doc);
    String out; serializeJson(doc, out);
    sendJson(200, out);
}

void WebUIManager::handleApiConfigPost() {
    String body = _server.arg("plain");
    if (body.isEmpty()) {
        sendJson(400, "{\"status\":\"error\",\"message\":\"empty body\"}");
        return;
    }
    DynamicJsonDocument doc(4096);
    auto err = deserializeJson(doc, body);
    if (err) {
        sendJson(400, "{\"status\":\"error\",\"message\":\"invalid json\"}");
        return;
    }
    bool ok = ProtocolV2::getInstance().jsonToConfig(doc);
    sendJson(ok ? 200 : 500, ok ? "{\"status\":\"ok\"}" : "{\"status\":\"error\"}");
}

void WebUIManager::handleSaveForm() {
    WiFiConfig cfg = networkManager.getConfig();
    if (_server.hasArg("ssid")) cfg.ssid = _server.arg("ssid");
    if (_server.hasArg("pass")) cfg.pass = _server.arg("pass");
    if (_server.hasArg("mode") && _server.arg("mode") == "static") {
        cfg.useDHCP = false;
        if (_server.hasArg("ip")) cfg.ip = _server.arg("ip");
        if (_server.hasArg("gw")) cfg.gw = _server.arg("gw");
        if (_server.hasArg("sn")) cfg.sn = _server.arg("sn");
        if (_server.hasArg("dns1")) cfg.dns1 = _server.arg("dns1");
        if (_server.hasArg("dns2")) cfg.dns2 = _server.arg("dns2");
    } else {
        cfg.useDHCP = true;
    }
    networkManager.applyConfig(cfg, true);
    _server.send(200, "text/html",
        "<html><body style='font-family:Arial;text-align:center;padding:30px'>"
        "<h3>Saved</h3><p>Reconnecting to WiFi...</p><p>If STA fails, setup AP will restart automatically.</p></body></html>");
    delay(300);
    networkManager.reconnectSTA();
}

void WebUIManager::handleCaptiveRedirect() {
    String target = String("http://") + WiFi.softAPIP().toString() + "/";
    _server.sendHeader("Location", target, true);
    _server.send(302, "text/plain", "");
}

void WebUIManager::handleNotFound() {
    if (networkManager.isAPMode()) {
        handleCaptiveRedirect();
    } else {
        _server.send(404, "text/plain", "Not found");
    }
}

String WebUIManager::buildWiFiSetupPage() {
    String page = FPSTR(PAGE_CONFIG_TEMPLATE);
    page.replace("%AP_SSID%", String(AP_SSID));
    page.replace("%AP_IP%", String(AP_IP_ADDR));
    return page;
}

void WebUIManager::sendJson(int code, const String& json) {
    _server.sendHeader("Cache-Control", "no-store");
    _server.send(code, "application/json", json);
}
