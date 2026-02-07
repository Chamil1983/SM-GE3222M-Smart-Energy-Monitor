/**
 * @file SystemManager.cpp
 * @brief Implementation of main system orchestrator
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "SystemManager.h"

SystemManager& SystemManager::getInstance() {
    static SystemManager instance;
    return instance;
}

bool SystemManager::init() {
    Serial.println("\n========================================");
    Serial.println("SM-GE3222M Smart Energy Monitor V2.0");
    Serial.println("System Initialization Starting");
    Serial.println("========================================\n");
    
    bootTime = millis();
    setState(SystemState::INITIALIZING);
    
    // Step 1: Initialize TimerManager (hardware timer and watchdog)
    Serial.println("[SystemManager] Step 1: Initializing TimerManager");
    if (!TimerManager::getInstance().init()) {
        ErrorHandler::getInstance().reportError(
            ErrorCode::WATCHDOG_RESET, 
            "SystemManager", 
            "TimerManager initialization failed"
        );
        setState(SystemState::ERROR);
        return false;
    }
    
    // Step 2: Initialize ErrorHandler (already initialized via singleton)
    Serial.println("[SystemManager] Step 2: ErrorHandler ready");
    
    // Step 3: Clear any previous errors
    ErrorHandler::getInstance().clearErrors();
    
    Serial.println("\n[SystemManager] Core subsystems initialized successfully");
    Serial.println("[SystemManager] Ready for subsystem initialization");
    Serial.println("[SystemManager] TaskScheduler ready for task registration\n");
    
    initialized = true;
    setState(SystemState::RUNNING);
    
    Serial.println("========================================");
    Serial.println("System Initialization Complete");
    Serial.printf("Uptime: %lu ms\n", getUptime());
    Serial.println("========================================\n");
    
    return true;
}

void SystemManager::run() {
    if (!initialized) {
        return;
    }
    
    // Feed watchdog periodically
    checkWatchdog();
    
    // Run task scheduler (executes all due tasks)
    if (currentState == SystemState::RUNNING) {
        TaskScheduler::getInstance().run();
    }
}

void SystemManager::setState(SystemState newState) {
    if (!isValidStateTransition(currentState, newState)) {
        Serial.printf("[SystemManager] Invalid state transition: %s -> %s\n",
                     getStateName(currentState), getStateName(newState));
        return;
    }
    
    SystemState oldState = currentState;
    currentState = newState;
    logStateChange(oldState, newState);
}

unsigned long SystemManager::getUptime() const {
    return millis() - bootTime;
}

void SystemManager::softReset() {
    Serial.println("\n[SystemManager] Performing soft reset");
    
    setState(SystemState::INITIALIZING);
    
    // Clear all tasks
    TaskScheduler::getInstance().clear();
    
    // Clear error history
    ErrorHandler::getInstance().clearErrors();
    
    // Reinitialize
    bootTime = millis();
    initialized = false;
    
    if (init()) {
        Serial.println("[SystemManager] Soft reset complete");
    } else {
        Serial.println("[SystemManager] Soft reset failed");
        setState(SystemState::ERROR);
    }
}

void SystemManager::enterMaintenanceMode() {
    Serial.println("[SystemManager] Entering maintenance mode");
    setState(SystemState::MAINTENANCE);
}

void SystemManager::exitMaintenanceMode() {
    Serial.println("[SystemManager] Exiting maintenance mode");
    setState(SystemState::RUNNING);
}

bool SystemManager::isValidStateTransition(SystemState fromState, SystemState toState) const {
    // Allow all transitions from BOOTING
    if (fromState == SystemState::BOOTING) {
        return true;
    }
    
    // Allow transitions to ERROR from any state
    if (toState == SystemState::ERROR) {
        return true;
    }
    
    // Allow transitions to MAINTENANCE from RUNNING
    if (fromState == SystemState::RUNNING && toState == SystemState::MAINTENANCE) {
        return true;
    }
    
    // Allow transitions back to RUNNING from MAINTENANCE or INITIALIZING
    if (toState == SystemState::RUNNING && 
        (fromState == SystemState::MAINTENANCE || fromState == SystemState::INITIALIZING)) {
        return true;
    }
    
    // Allow BOOTING -> INITIALIZING
    if (fromState == SystemState::BOOTING && toState == SystemState::INITIALIZING) {
        return true;
    }
    
    // Allow INITIALIZING -> RUNNING
    if (fromState == SystemState::INITIALIZING && toState == SystemState::RUNNING) {
        return true;
    }
    
    return false;
}

void SystemManager::logStateChange(SystemState oldState, SystemState newState) {
    Serial.printf("[SystemManager] State change: %s -> %s\n",
                 getStateName(oldState), getStateName(newState));
    
    // Log to error handler if entering ERROR state
    if (newState == SystemState::ERROR) {
        ErrorHandler::getInstance().reportError(
            ErrorCode::NONE,
            "SystemManager",
            "System entered ERROR state"
        );
    }
}

const char* SystemManager::getStateName(SystemState state) {
    switch (state) {
        case SystemState::BOOTING: return "BOOTING";
        case SystemState::INITIALIZING: return "INITIALIZING";
        case SystemState::RUNNING: return "RUNNING";
        case SystemState::ERROR: return "ERROR";
        case SystemState::MAINTENANCE: return "MAINTENANCE";
        default: return "UNKNOWN";
    }
}

void SystemManager::checkWatchdog() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - lastWatchdogFeed >= WATCHDOG_FEED_INTERVAL) {
        TimerManager::getInstance().feedWatchdog();
        lastWatchdogFeed = currentMillis;
    }
}
