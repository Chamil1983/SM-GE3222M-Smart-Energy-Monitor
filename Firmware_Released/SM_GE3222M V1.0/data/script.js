var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
// Init web socket when the page loads
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
    initConfig();
}

function initConfig() {

       document.getElementById('setCallibration').addEventListener('click', setCallibration);

  }

  function setCallibration(){

        var configdata = {


    //Send Calibration Register Data
    PoffsetAtxt: document.getElementById("PoffsetAtxt").value,
    QoffsetAtxt: document.getElementById("QoffsetAtxt").value,
    POffsetBtxt: document.getElementById("POffsetBtxt").value,
    QOffsetBtxt: document.getElementById("QOffsetBtxt").value,
    POffsetCtxt: document.getElementById("POffsetCtxt").value,
    QOffsetCtxt: document.getElementById("QOffsetCtxt").value,
    GainAtxt: document.getElementById("GainAtxt").value,
    PhiAtxt: document.getElementById("PhiAtxt").value,
    GainBtxt: document.getElementById("GainBtxt").value,
    PhiBtxt: document.getElementById("PhiBtxt").value,
    GainCtxt: document.getElementById("GainCtxt").value,
    PhiCtxt: document.getElementById("PhiCtxt").value,
    CS1txt: document.getElementById("CS1txt").value,

    //Fundamental/ Harmonic Energy Calibration register Data
    POffsetAFtxt: document.getElementById("POffsetAFtxt").value,
    POffsetBFtxt: document.getElementById("POffsetBFtxt").value,
    POffsetCFtxt: document.getElementById("POffsetCFtxt").value,
    PGainAFtxt: document.getElementById("PGainAFtxt").value,
    PGainBFtxt: document.getElementById("PGainBFtxt").value,
    PGainCFtxt: document.getElementById("PGainCFtxt").value,
    CS2txt: document.getElementById("CS2txt").value,

    //Measurement Calibration Register Data 
    UgainAtxt: document.getElementById("UgainAtxt").value, 
    IgainAtxt: document.getElementById("IgainAtxt").value,
    UoffsetAtxt: document.getElementById("UoffsetAtxt").value,
    IoffsetAtxtx: document.getElementById("IoffsetAtxtx").value,
    UgainBtxt: document.getElementById("UgainBtxt").value,
    IgainBtxt: document.getElementById("IgainBtxt").value,
    UoffsetBtxt: document.getElementById("UoffsetBtxt").value,
    IoffsetBtxt: document.getElementById("IoffsetBtxt").value,
    UgainCtxt: document.getElementById("UgainCtxt").value,
    IgainCtxt: document.getElementById("IgainCtxt").value,
    UoffsetCtxt: document.getElementById("UoffsetCtxt").value,
    IoffsetCtxt: document.getElementById("IoffsetCtxt").value,
    IgainNtxt: document.getElementById("IgainNtxt").value,
    IoffsetNtxt: document.getElementById("IoffsetNtxt").value,
    CS3txt: document.getElementById("CS3txt").value

        };
        websocket.send(JSON.stringify(configdata));


  }


function getSystemInfo(){
    websocket.send("getmeterInfo");
}

function getConfigInfo(){
    websocket.send("getmeterconfig1");
    websocket.send("getmeterconfig2");
    websocket.send("getmeterconfig3");
}

function getMtrReadings(){
    websocket.send("getreadings");

}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

// When websocket is established, call the getSystemInfo() function
function onOpen(event) {
    console.log('Connection opened');
    getSystemInfo();
    getConfigInfo();
    getMtrReadings();
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

// Function that receives the message from the ESP32 with the readings
function onMessage(event) {
    console.log(event.data);
    var myObj = JSON.parse(event.data);
    var keys = Object.keys(myObj);

    for (var i = 0; i < keys.length; i++){
        var key = keys[i];
        document.getElementById(key).innerHTML = myObj[key];
    }
}

