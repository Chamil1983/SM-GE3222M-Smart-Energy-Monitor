// SM-GE3222M Web Application
// ES6+ Frontend JavaScript with WebSocket and REST API

// ============================================================================
// Global State
// ============================================================================
let ws = null;
let reconnectTimeout = null;
let chartInstance = null;
let gaugeCharts = {};
let csrfToken = '';

const powerHistory = {
    labels: [],
    dataA: [],
    dataB: [],
    dataC: [],
    maxPoints: 60
};

// ============================================================================
// Theme Management
// ============================================================================
function initTheme() {
    const savedTheme = localStorage.getItem('theme') || 'light';
    document.documentElement.setAttribute('data-theme', savedTheme);
    updateThemeIcon(savedTheme);
}

function toggleTheme() {
    const currentTheme = document.documentElement.getAttribute('data-theme');
    const newTheme = currentTheme === 'dark' ? 'light' : 'dark';
    document.documentElement.setAttribute('data-theme', newTheme);
    localStorage.setItem('theme', newTheme);
    updateThemeIcon(newTheme);
}

function updateThemeIcon(theme) {
    const icon = document.querySelector('.theme-icon');
    if (icon) {
        icon.textContent = theme === 'dark' ? '☀️' : '🌙';
    }
}

// ============================================================================
// WebSocket Management
// ============================================================================
function connectWebSocket() {
    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const wsUrl = `${protocol}//${window.location.hostname}/ws`;
    
    ws = new WebSocket(wsUrl);
    
    ws.onopen = () => {
        console.log('WebSocket connected');
        updateConnectionStatus('connected');
    };
    
    ws.onmessage = (event) => {
        try {
            const data = JSON.parse(event.data);
            updateDashboard(data);
        } catch (error) {
            console.error('Error parsing WebSocket message:', error);
        }
    };
    
    ws.onerror = (error) => {
        console.error('WebSocket error:', error);
        updateConnectionStatus('error');
    };
    
    ws.onclose = () => {
        console.log('WebSocket disconnected');
        updateConnectionStatus('disconnected');
        
        if (reconnectTimeout) {
            clearTimeout(reconnectTimeout);
        }
        reconnectTimeout = setTimeout(connectWebSocket, 3000);
    };
}

function updateConnectionStatus(status) {
    const wsStatus = document.getElementById('wsStatus');
    const wsLabel = document.getElementById('wsLabel');
    
    if (wsStatus) {
        wsStatus.className = 'status-icon';
        if (status === 'connected') {
            wsStatus.classList.add('connected');
            if (wsLabel) wsLabel.textContent = 'Live';
        } else {
            wsStatus.classList.add('disconnected');
            if (wsLabel) wsLabel.textContent = 'Offline';
        }
    }
}

// ============================================================================
// Dashboard Updates
// ============================================================================
function updateDashboard(data) {
    if (!data) return;
    
    // Update phase measurements
    if (data.phases) {
        updatePhase('A', data.phases.A || {});
        updatePhase('B', data.phases.B || {});
        updatePhase('C', data.phases.C || {});
    }
    
    // Update summary
    if (data.summary) {
        updateElement('totalPower', (data.summary.totalPower / 1000).toFixed(2));
        updateElement('energyToday', (data.summary.energyToday / 1000).toFixed(2));
        updateElement('totalEnergy', (data.summary.totalEnergy / 1000).toFixed(2));
    }
    
    // Update system status
    if (data.status) {
        updateSystemStatus(data.status);
    }
    
    // Update chart
    if (data.phases && chartInstance) {
        updatePowerChart(data.phases);
    }
    
    // Update timestamp
    updateElement('lastUpdate', new Date().toLocaleTimeString());
}

function updatePhase(phase, data) {
    const voltage = data.voltage || 0;
    const current = data.current || 0;
    const power = data.power || 0;
    const powerFactor = data.powerFactor || 0;
    
    updateElement(`voltage${phase}`, voltage.toFixed(1));
    updateElement(`current${phase}`, current.toFixed(2));
    updateElement(`power${phase}`, power.toFixed(2));
    updateElement(`pf${phase}`, powerFactor.toFixed(2));
    
    // Update gauge
    updateGauge(phase, voltage);
}

function updateElement(id, value) {
    const element = document.getElementById(id);
    if (element) {
        element.textContent = value;
    }
}

function updateSystemStatus(status) {
    updateStatusIndicator('wifiStatus', 'wifiLabel', status.wifi, 'WiFi');
    updateStatusIndicator('mqttStatus', 'mqttLabel', status.mqtt, 'MQTT');
    updateStatusIndicator('modbusStatus', 'modbusLabel', status.modbus, 'Modbus');
}

function updateStatusIndicator(iconId, labelId, connected, label) {
    const icon = document.getElementById(iconId);
    const labelElem = document.getElementById(labelId);
    
    if (icon) {
        icon.className = 'status-icon';
        icon.classList.add(connected ? 'connected' : 'disconnected');
    }
    
    if (labelElem) {
        labelElem.textContent = label;
    }
}

// ============================================================================
// Chart Management
// ============================================================================
function initCharts() {
    initVoltageGauges();
    initPowerChart();
}

function initVoltageGauges() {
    const phases = ['A', 'B', 'C'];
    
    phases.forEach(phase => {
        const canvas = document.getElementById(`gaugeVoltage${phase}`);
        if (!canvas) return;
        
        const ctx = canvas.getContext('2d');
        gaugeCharts[phase] = new Chart(ctx, {
            type: 'doughnut',
            data: {
                datasets: [{
                    data: [0, 100],
                    backgroundColor: [
                        getComputedStyle(document.documentElement).getPropertyValue('--primary-color'),
                        getComputedStyle(document.documentElement).getPropertyValue('--border-color')
                    ],
                    borderWidth: 0
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: true,
                cutout: '70%',
                circumference: 270,
                rotation: 225,
                plugins: {
                    legend: { display: false },
                    tooltip: { enabled: false }
                }
            }
        });
    });
}

function updateGauge(phase, voltage) {
    const gauge = gaugeCharts[phase];
    if (!gauge) return;
    
    const maxVoltage = 300;
    const percentage = Math.min((voltage / maxVoltage) * 100, 100);
    
    gauge.data.datasets[0].data = [percentage, 100 - percentage];
    gauge.update('none');
}

function initPowerChart() {
    const canvas = document.getElementById('powerChart');
    if (!canvas) return;
    
    const ctx = canvas.getContext('2d');
    chartInstance = new Chart(ctx, {
        type: 'line',
        data: {
            labels: powerHistory.labels,
            datasets: [
                {
                    label: 'Phase A',
                    data: powerHistory.dataA,
                    borderColor: '#FF6384',
                    backgroundColor: 'rgba(255, 99, 132, 0.1)',
                    tension: 0.4,
                    fill: true
                },
                {
                    label: 'Phase B',
                    data: powerHistory.dataB,
                    borderColor: '#36A2EB',
                    backgroundColor: 'rgba(54, 162, 235, 0.1)',
                    tension: 0.4,
                    fill: true
                },
                {
                    label: 'Phase C',
                    data: powerHistory.dataC,
                    borderColor: '#FFCE56',
                    backgroundColor: 'rgba(255, 206, 86, 0.1)',
                    tension: 0.4,
                    fill: true
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            interaction: {
                mode: 'index',
                intersect: false
            },
            plugins: {
                legend: {
                    display: true,
                    position: 'top'
                }
            },
            scales: {
                y: {
                    beginAtZero: true,
                    title: {
                        display: true,
                        text: 'Power (W)'
                    }
                },
                x: {
                    title: {
                        display: true,
                        text: 'Time'
                    }
                }
            }
        }
    });
}

function updatePowerChart(phases) {
    if (!chartInstance) return;
    
    const now = new Date().toLocaleTimeString();
    
    powerHistory.labels.push(now);
    powerHistory.dataA.push(phases.A?.power || 0);
    powerHistory.dataB.push(phases.B?.power || 0);
    powerHistory.dataC.push(phases.C?.power || 0);
    
    if (powerHistory.labels.length > powerHistory.maxPoints) {
        powerHistory.labels.shift();
        powerHistory.dataA.shift();
        powerHistory.dataB.shift();
        powerHistory.dataC.shift();
    }
    
    chartInstance.update('none');
}

// ============================================================================
// API Helper Functions
// ============================================================================
async function apiRequest(url, options = {}) {
    try {
        const response = await fetch(url, {
            ...options,
            headers: {
                'Content-Type': 'application/json',
                ...options.headers
            }
        });
        
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}: ${response.statusText}`);
        }
        
        return await response.json();
    } catch (error) {
        console.error('API request failed:', error);
        showToast(error.message, 'error');
        throw error;
    }
}

async function getConfig() {
    return await apiRequest('/api/v1/config');
}

async function saveConfig(config) {
    return await apiRequest('/api/v1/config', {
        method: 'PUT',
        body: JSON.stringify(config)
    });
}

async function getMeasurements() {
    return await apiRequest('/api/v1/measurements');
}

async function getStatus() {
    return await apiRequest('/api/v1/status');
}

// ============================================================================
// Configuration Management
// ============================================================================
async function loadConfiguration() {
    try {
        const config = await getConfig();
        populateConfigForm(config);
        showToast('Configuration loaded', 'success');
    } catch (error) {
        showToast('Failed to load configuration', 'error');
    }
}

function populateConfigForm(config) {
    if (!config) return;
    
    // WiFi settings
    setFormValue('wifiSsid', config.wifi?.ssid);
    setFormValue('wifiDhcp', config.wifi?.dhcp);
    setFormValue('wifiIp', config.wifi?.ip);
    setFormValue('wifiGateway', config.wifi?.gateway);
    setFormValue('wifiSubnet', config.wifi?.subnet);
    
    // MQTT settings
    setFormValue('mqttServer', config.mqtt?.server);
    setFormValue('mqttPort', config.mqtt?.port);
    setFormValue('mqttUser', config.mqtt?.user);
    setFormValue('mqttTopic', config.mqtt?.topic);
    setFormValue('mqttInterval', config.mqtt?.interval);
    
    // Modbus settings
    setFormValue('modbusSlaveId', config.modbus?.slaveId);
    setFormValue('modbusBaud', config.modbus?.baudRate);
    setFormValue('modbusParity', config.modbus?.parity);
    setFormValue('modbusTcpPort', config.modbus?.tcpPort);
    setFormValue('modbusTcpMaxConn', config.modbus?.maxConnections);
    
    // Calibration
    if (config.calibration) {
        const phases = ['A', 'B', 'C'];
        phases.forEach(phase => {
            const cal = config.calibration[`phase${phase}`] || {};
            setFormValue(`voltageGain${phase}`, cal.voltageGain);
            setFormValue(`currentGain${phase}`, cal.currentGain);
            setFormValue(`voltageOffset${phase}`, cal.voltageOffset);
            setFormValue(`currentOffset${phase}`, cal.currentOffset);
        });
    }
    
    // System settings
    setFormValue('logLevel', config.system?.logLevel);
    setFormValue('lcdAutoScroll', config.system?.lcdAutoScroll);
    setFormValue('ntpServer', config.system?.ntpServer);
    
    // Get CSRF token
    if (config.csrfToken) {
        csrfToken = config.csrfToken;
        setFormValue('csrfToken', csrfToken);
    }
    
    toggleStaticIpFields();
}

function setFormValue(id, value) {
    const element = document.getElementById(id);
    if (!element) return;
    
    if (element.type === 'checkbox') {
        element.checked = value === true || value === 'true';
    } else if (value !== undefined && value !== null) {
        element.value = value;
    }
}

function getFormValue(id) {
    const element = document.getElementById(id);
    if (!element) return null;
    
    if (element.type === 'checkbox') {
        return element.checked;
    }
    return element.value;
}

function toggleStaticIpFields() {
    const dhcpEnabled = document.getElementById('wifiDhcp')?.checked;
    const staticFields = document.getElementById('staticIpFields');
    
    if (staticFields) {
        staticFields.style.display = dhcpEnabled ? 'none' : 'block';
    }
}

async function handleConfigSubmit(event) {
    event.preventDefault();
    
    const config = {
        wifi: {
            ssid: getFormValue('wifiSsid'),
            password: getFormValue('wifiPassword'),
            dhcp: getFormValue('wifiDhcp'),
            ip: getFormValue('wifiIp'),
            gateway: getFormValue('wifiGateway'),
            subnet: getFormValue('wifiSubnet')
        },
        mqtt: {
            server: getFormValue('mqttServer'),
            port: parseInt(getFormValue('mqttPort')),
            user: getFormValue('mqttUser'),
            password: getFormValue('mqttPassword'),
            topic: getFormValue('mqttTopic'),
            interval: parseInt(getFormValue('mqttInterval'))
        },
        modbus: {
            slaveId: parseInt(getFormValue('modbusSlaveId')),
            baudRate: parseInt(getFormValue('modbusBaud')),
            parity: getFormValue('modbusParity'),
            tcpPort: parseInt(getFormValue('modbusTcpPort')),
            maxConnections: parseInt(getFormValue('modbusTcpMaxConn'))
        },
        calibration: {
            phaseA: {
                voltageGain: parseFloat(getFormValue('voltageGainA')),
                currentGain: parseFloat(getFormValue('currentGainA')),
                voltageOffset: parseFloat(getFormValue('voltageOffsetA')),
                currentOffset: parseFloat(getFormValue('currentOffsetA'))
            },
            phaseB: {
                voltageGain: parseFloat(getFormValue('voltageGainB')),
                currentGain: parseFloat(getFormValue('currentGainB')),
                voltageOffset: parseFloat(getFormValue('voltageOffsetB')),
                currentOffset: parseFloat(getFormValue('currentOffsetB'))
            },
            phaseC: {
                voltageGain: parseFloat(getFormValue('voltageGainC')),
                currentGain: parseFloat(getFormValue('currentGainC')),
                voltageOffset: parseFloat(getFormValue('voltageOffsetC')),
                currentOffset: parseFloat(getFormValue('currentOffsetC'))
            }
        },
        system: {
            logLevel: parseInt(getFormValue('logLevel')),
            lcdAutoScroll: getFormValue('lcdAutoScroll'),
            ntpServer: getFormValue('ntpServer')
        },
        csrfToken: csrfToken
    };
    
    try {
        await saveConfig(config);
        showToast('Configuration saved successfully', 'success');
    } catch (error) {
        showToast('Failed to save configuration', 'error');
    }
}

// ============================================================================
// Firmware Upload
// ============================================================================
async function handleFirmwareUpload() {
    const fileInput = document.getElementById('firmwareFile');
    const file = fileInput?.files[0];
    
    if (!file) {
        showToast('Please select a firmware file', 'warning');
        return;
    }
    
    if (!file.name.endsWith('.bin')) {
        showToast('Invalid file type. Please select a .bin file', 'error');
        return;
    }
    
    const formData = new FormData();
    formData.append('firmware', file);
    
    const progressBar = document.getElementById('uploadProgress');
    const progressFill = progressBar?.querySelector('.progress-fill');
    
    if (progressBar) progressBar.style.display = 'block';
    
    try {
        const xhr = new XMLHttpRequest();
        
        xhr.upload.addEventListener('progress', (e) => {
            if (e.lengthComputable && progressFill) {
                const percent = (e.loaded / e.total) * 100;
                progressFill.style.width = percent + '%';
            }
        });
        
        xhr.addEventListener('load', () => {
            if (xhr.status === 200) {
                showToast('Firmware uploaded successfully. Device will reboot.', 'success');
                setTimeout(() => {
                    window.location.reload();
                }, 5000);
            } else {
                showToast('Firmware upload failed', 'error');
            }
            if (progressBar) progressBar.style.display = 'none';
        });
        
        xhr.addEventListener('error', () => {
            showToast('Firmware upload failed', 'error');
            if (progressBar) progressBar.style.display = 'none';
        });
        
        xhr.open('POST', '/api/v1/firmware');
        xhr.send(formData);
        
    } catch (error) {
        showToast('Firmware upload failed', 'error');
        if (progressBar) progressBar.style.display = 'none';
    }
}

// ============================================================================
// Factory Reset
// ============================================================================
async function handleFactoryReset() {
    const confirmed = await showConfirmDialog(
        'Factory Reset',
        'Are you sure you want to reset all settings to factory defaults? This action cannot be undone.'
    );
    
    if (!confirmed) return;
    
    try {
        await apiRequest('/api/v1/factory-reset', { method: 'POST' });
        showToast('Factory reset completed. Device will reboot.', 'success');
        setTimeout(() => {
            window.location.href = '/';
        }, 3000);
    } catch (error) {
        showToast('Factory reset failed', 'error');
    }
}

// ============================================================================
// UI Utilities
// ============================================================================
function showToast(message, type = 'info') {
    const toast = document.getElementById('toast');
    if (!toast) return;
    
    toast.textContent = message;
    toast.className = `toast ${type}`;
    toast.classList.add('show');
    
    setTimeout(() => {
        toast.classList.remove('show');
    }, 3000);
}

function showConfirmDialog(title, message) {
    return new Promise((resolve) => {
        const dialog = document.getElementById('confirmDialog');
        const titleElem = document.getElementById('confirmTitle');
        const messageElem = document.getElementById('confirmMessage');
        const yesBtn = document.getElementById('confirmYes');
        const noBtn = document.getElementById('confirmNo');
        
        if (!dialog) {
            resolve(false);
            return;
        }
        
        if (titleElem) titleElem.textContent = title;
        if (messageElem) messageElem.textContent = message;
        
        dialog.style.display = 'flex';
        
        const handleYes = () => {
            dialog.style.display = 'none';
            cleanup();
            resolve(true);
        };
        
        const handleNo = () => {
            dialog.style.display = 'none';
            cleanup();
            resolve(false);
        };
        
        const cleanup = () => {
            yesBtn?.removeEventListener('click', handleYes);
            noBtn?.removeEventListener('click', handleNo);
        };
        
        yesBtn?.addEventListener('click', handleYes);
        noBtn?.addEventListener('click', handleNo);
    });
}

async function handleLogout() {
    try {
        await apiRequest('/api/v1/logout', { method: 'POST' });
        window.location.href = '/login.html';
    } catch (error) {
        showToast('Logout failed', 'error');
    }
}

// ============================================================================
// Event Listeners
// ============================================================================
function attachEventListeners() {
    // Theme toggle
    const themeToggle = document.getElementById('themeToggle');
    if (themeToggle) {
        themeToggle.addEventListener('click', toggleTheme);
    }
    
    // Logout
    const logoutBtn = document.getElementById('logoutBtn');
    if (logoutBtn) {
        logoutBtn.addEventListener('click', handleLogout);
    }
    
    // Configuration form
    const configForm = document.getElementById('configForm');
    if (configForm) {
        configForm.addEventListener('submit', handleConfigSubmit);
        
        const dhcpCheckbox = document.getElementById('wifiDhcp');
        if (dhcpCheckbox) {
            dhcpCheckbox.addEventListener('change', toggleStaticIpFields);
        }
        
        const reloadBtn = document.getElementById('reloadConfig');
        if (reloadBtn) {
            reloadBtn.addEventListener('click', loadConfiguration);
        }
        
        const factoryResetBtn = document.getElementById('factoryReset');
        if (factoryResetBtn) {
            factoryResetBtn.addEventListener('click', handleFactoryReset);
        }
        
        const uploadFirmwareBtn = document.getElementById('uploadFirmware');
        if (uploadFirmwareBtn) {
            uploadFirmwareBtn.addEventListener('click', handleFirmwareUpload);
        }
        
        const testCalibrationBtn = document.getElementById('testCalibration');
        if (testCalibrationBtn) {
            testCalibrationBtn.addEventListener('click', async () => {
                try {
                    const data = await getMeasurements();
                    showToast('Check console for live measurements', 'info');
                    console.log('Current measurements:', data);
                } catch (error) {
                    showToast('Failed to get measurements', 'error');
                }
            });
        }
    }
}

// ============================================================================
// Initialization
// ============================================================================
function init() {
    console.log('Initializing SM-GE3222M Web Application');
    
    initTheme();
    attachEventListeners();
    
    // Initialize charts on dashboard
    if (document.getElementById('powerChart')) {
        initCharts();
        connectWebSocket();
    }
    
    // Load configuration on config page
    if (document.getElementById('configForm')) {
        loadConfiguration();
    }
}

// Start application when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
} else {
    init();
}
