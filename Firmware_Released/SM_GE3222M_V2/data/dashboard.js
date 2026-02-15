// WebSocket connection
let ws = null;
let pollTimer = null;

function startPolling() {
    if (pollTimer) return;
    pollTimer = setInterval(fetchInitialData, 1000);

    // Show that we are updating via HTTP while WS is down.
    const wsStatus = document.getElementById('wsStatus');
    const wsStatusText = document.getElementById('wsStatusText');
    if (wsStatus) {
        wsStatus.className = 'status-indicator connected';
    }
    if (wsStatusText) {
        wsStatusText.textContent = 'HTTP polling';
    }
}

function stopPolling() {
    if (!pollTimer) return;
    clearInterval(pollTimer);
    pollTimer = null;
}

let reconnectInterval = null;
let reconnectDelay = 1000;
const maxReconnectDelay = 30000;

// Initialize on page load
document.addEventListener('DOMContentLoaded', function() {
    initWebSocket();
    fetchInitialData();
});

// Initialize WebSocket connection
function initWebSocket() {
    const wsUrl = `ws://${window.location.hostname}/ws`;
    
    try {
        ws = new WebSocket(wsUrl);
        
        ws.onopen = function() {
            console.log('WebSocket Connected');
            updateConnectionStatus(true);
            reconnectDelay = 1000; // Reset delay on successful connection
            
            // Request initial data
            ws.send('getreadings');
        };
        
        ws.onmessage = function(event) {
            try {
                const data = JSON.parse(event.data);
                updateDashboard(data);
            } catch (error) {
                console.error('Error parsing WebSocket message:', error);
            }
        };
        
        ws.onerror = function(error) {
            console.error('WebSocket Error:', error);
            updateConnectionStatus(false);
        };
        
        ws.onclose = function() {
            console.log('WebSocket Disconnected');
            updateConnectionStatus(false);
            scheduleReconnect();
        };
    } catch (error) {
        console.error('WebSocket Connection Error:', error);
        updateConnectionStatus(false);
        scheduleReconnect();
    }
}

// Schedule reconnection with exponential backoff
function scheduleReconnect() {
    if (reconnectInterval) return;
    
    reconnectInterval = setTimeout(function() {
        console.log('Attempting to reconnect...');
        reconnectInterval = null;
        reconnectDelay = Math.min(reconnectDelay * 1.5, maxReconnectDelay);
        initWebSocket();
    }, reconnectDelay);
}

// Update connection status indicator
function updateConnectionStatus(connected) {
    const statusIndicator = document.getElementById('wsStatus');
    const statusText = document.getElementById('wsStatusText');
    
    if (connected) {
        statusIndicator.className = 'status-indicator connected';
        statusText.textContent = 'Connected';
    } else {
        statusIndicator.className = 'status-indicator disconnected';
        statusText.textContent = 'Disconnected';
    }
}

// Fetch initial data via HTTP API
function fetchInitialData() {
    fetch('/api/meter')
        .then(response => response.json())
        .then(data => {
            updateDashboard(data);
        })
        .catch(error => {
            console.log('HTTP API not available, waiting for WebSocket data');
        });
}

// Update all dashboard elements with new data
function updateDashboard(data) {
    // Update timestamp
    const now = new Date();
    document.getElementById('lastUpdate').textContent = now.toLocaleTimeString();
    
    // Voltage (RMS values)
    updateElement('voltageA', data.UrmsA, 1);
    updateElement('voltageB', data.UrmsB, 1);
    updateElement('voltageC', data.UrmsC, 1);
    
    // Update voltage gauges
    updateGauge('gaugeA', data.UrmsA || 0, 230, 'phase-a');
    updateGauge('gaugeB', data.UrmsB || 0, 230, 'phase-b');
    updateGauge('gaugeC', data.UrmsC || 0, 230, 'phase-c');
    
    // Current (RMS values)
    updateElement('currentA', data.IrmsA, 2);
    updateElement('currentB', data.IrmsB, 2);
    updateElement('currentC', data.IrmsC, 2);
    
    // Active Power
    updateElement('powerA', data.PmeanA, 2);
    updateElement('powerB', data.PmeanB, 2);
    updateElement('powerC', data.PmeanC, 2);
    updateElement('powerTotal', data.PmeanT, 2);
    
    // Reactive Power
    updateElement('reactivePowerA', data.QmeanA, 2);
    updateElement('reactivePowerB', data.QmeanB, 2);
    updateElement('reactivePowerC', data.QmeanC, 2);
    updateElement('reactivePowerTotal', data.QmeanT, 2);
    
    // Apparent Power
    updateElement('apparentPowerA', data.SmeanA, 2);
    updateElement('apparentPowerB', data.SmeanB, 2);
    updateElement('apparentPowerC', data.SmeanC, 2);
    updateElement('apparentPowerTotal', data.SAmeanT, 2);
    
    // Power Factor
    updateElement('pfA', data.PFmeanA, 2);
    updateElement('pfB', data.PFmeanB, 2);
    updateElement('pfC', data.PFmeanC, 2);
    updateElement('pfTotal', data.PFmeanT, 2);
    
    // System info
    updateElement('frequency', data.Freq, 2);
    updateElement('temperature', data.Temp, 1);
    
    // Energy counters
    updateElement('importEnergy', data.APenergyT, 3);
    updateElement('exportEnergy', data.ANenergyT, 3);
    updateElement('reactiveImport', data.RPenergyT, 3);
    updateElement('reactiveExport', data.RNenergyT, 3);
    
    // System status
    if (data.uptime !== undefined) {
        document.getElementById('uptime').textContent = formatUptime(data.uptime);
    }
    if (data.freeHeap !== undefined) {
        document.getElementById('freeHeap').textContent = formatBytes(data.freeHeap);
    }
    if (data.rssi !== undefined) {
        document.getElementById('rssi').textContent = data.rssi + ' dBm';
    }
    if (data.ipaddress !== undefined) {
        document.getElementById('ipAddress').textContent = data.ipaddress;
    }
}

// Update a single element with formatted value
function updateElement(id, value, decimals) {
    const element = document.getElementById(id);
    if (element && value !== undefined && value !== null) {
        const numValue = parseFloat(value);
        if (!isNaN(numValue)) {
            element.textContent = numValue.toFixed(decimals);
        }
    }
}

// Update voltage gauge visualization
function updateGauge(gaugeId, value, maxValue, phaseClass) {
    const gauge = document.getElementById(gaugeId);
    if (!gauge) return;
    
    const percentage = Math.min(Math.max(value / maxValue, 0), 1);
    const angle = percentage * 180;
    const radians = (angle - 90) * (Math.PI / 180);
    
    const centerX = 100;
    const centerY = 100;
    const radius = 80;
    
    const endX = centerX + radius * Math.cos(radians);
    const endY = centerY + radius * Math.sin(radians);
    
    const largeArc = angle > 180 ? 1 : 0;
    
    const pathData = `M 20 100 A 80 80 0 ${largeArc} 1 ${endX} ${endY}`;
    gauge.setAttribute('d', pathData);
}

// Format uptime (seconds to HH:MM:SS)
function formatUptime(seconds) {
    const hrs = Math.floor(seconds / 3600);
    const mins = Math.floor((seconds % 3600) / 60);
    const secs = seconds % 60;
    return `${pad(hrs)}:${pad(mins)}:${pad(secs)}`;
}

// Pad number with leading zero
function pad(num) {
    return num.toString().padStart(2, '0');
}

// Format bytes to human-readable format
function formatBytes(bytes) {
    if (bytes < 1024) return bytes + ' B';
    if (bytes < 1048576) return (bytes / 1024).toFixed(1) + ' KB';
    return (bytes / 1048576).toFixed(1) + ' MB';
}

// Send command to device
function sendCommand(command) {
    if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send(command);
    } else {
        console.error('WebSocket not connected');
    }
}

// Cleanup on page unload
window.addEventListener('beforeunload', function() {
    if (reconnectInterval) {
        clearTimeout(reconnectInterval);
    }
    if (ws) {
        ws.close();
    }
});
