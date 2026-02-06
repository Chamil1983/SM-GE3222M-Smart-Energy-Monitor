/**
 * @file SystemManager.h
 * @brief Main system orchestrator
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Coordinates all subsystems and manages system lifecycle
 */

#pragma once

#include <Arduino.h>
#include "../../include/GlobalTypes.h"
#include "TaskScheduler.h"
#include "ErrorHandler.h"
#include "TimerManager.h"

/**
 * @class SystemManager
 * @brief Main orchestrator that coordinates all subsystems
 * 
 * Singleton class that manages system initialization, state transitions,
 * and coordinates the operation of all subsystems through the TaskScheduler.
 */
class SystemManager {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to SystemManager instance
     */
    static SystemManager& getInstance();
    
    /**
     * @brief Initialize all subsystems
     * @return true if initialization successful
     * 
     * Initializes subsystems in the correct order:
     * 1. TimerManager (watchdog and timers)
     * 2. ErrorHandler (error management)
     * 3. Storage subsystems
     * 4. HAL layer
     * 5. Communication subsystems
     * 6. TaskScheduler (task registration)
     */
    bool init();
    
    /**
     * @brief Main system loop
     * 
     * Should be called continuously in the main loop.
     * Delegates to TaskScheduler and manages watchdog.
     */
    void run();
    
    /**
     * @brief Get current system state
     * @return Current SystemState
     */
    SystemState getState() const { return currentState; }
    
    /**
     * @brief Set system state
     * @param newState New system state
     * 
     * Validates state transition and logs state change
     */
    void setState(SystemState newState);
    
    /**
     * @brief Get system uptime
     * @return Uptime in milliseconds
     */
    unsigned long getUptime() const;
    
    /**
     * @brief Check if system is initialized
     * @return true if system is initialized
     */
    bool isInitialized() const { return initialized; }
    
    /**
     * @brief Perform soft reset
     * 
     * Reinitializes all subsystems without hardware reset
     */
    void softReset();
    
    /**
     * @brief Get boot time
     * @return Boot timestamp in milliseconds
     */
    unsigned long getBootTime() const { return bootTime; }
    
    /**
     * @brief Enter maintenance mode
     * 
     * Pauses normal operation for maintenance tasks
     */
    void enterMaintenanceMode();
    
    /**
     * @brief Exit maintenance mode
     * 
     * Resumes normal operation after maintenance
     */
    void exitMaintenanceMode();

private:
    SystemManager() : currentState(SystemState::BOOTING), 
                      initialized(false), 
                      bootTime(0),
                      lastWatchdogFeed(0) {}
    
    SystemManager(const SystemManager&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;
    
    SystemState currentState;
    bool initialized;
    unsigned long bootTime;
    unsigned long lastWatchdogFeed;
    
    static const unsigned long WATCHDOG_FEED_INTERVAL = 1000;  // Feed every 1 second
    
    /**
     * @brief Validate state transition
     * @param fromState Current state
     * @param toState Desired state
     * @return true if transition is valid
     */
    bool isValidStateTransition(SystemState fromState, SystemState toState) const;
    
    /**
     * @brief Log state change
     * @param oldState Previous state
     * @param newState New state
     */
    void logStateChange(SystemState oldState, SystemState newState);
    
    /**
     * @brief Get state name as string
     * @param state System state
     * @return String representation of state
     */
    static const char* getStateName(SystemState state);
    
    /**
     * @brief Check and feed watchdog if needed
     */
    void checkWatchdog();
};
