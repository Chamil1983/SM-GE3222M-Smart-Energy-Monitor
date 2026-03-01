/**
 * @file TaskManager.h
 * @brief FreeRTOS task management for SM-GE3222M V2.0
 * 
 * Singleton pattern managing system tasks with proper core affinity,
 * priorities, and stack sizes for ESP32 dual-core architecture.
 * 
 * Task Architecture:
 * Core 1 (Energy & Modbus):
 *   - EnergyTask: Read ATM90E36, update meter (500ms, P5)
 *   - AccumulatorTask: Update energy accumulator, auto-save (1000ms, P4)
 * 
 * Core 0 (Communications & Diagnostics):
 *   - DiagnosticsTask: System monitoring, logging (5000ms, P1)
 */

#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "DataTypes.h"
#include "Logger.h"

class TaskManager {
public:
    static TaskManager& getInstance();
    
    bool createAllTasks();

    void stopAllTasks();
    
    bool isRunning() const { return _tasksRunning; }
    
    TaskHandle_t getEnergyTaskHandle() const { return _energyTask; }
    TaskHandle_t getAccumulatorTaskHandle() const { return _accumulatorTask; }
    TaskHandle_t getModbusTaskHandle() const { return _modbusTask; }
    TaskHandle_t getTCPServerTaskHandle() const { return _tcpServerTask; }
    TaskHandle_t getMQTTTaskHandle() const { return _mqttTask; }
    bool isTCPServerTaskRunning() const { return _tcpServerTask != nullptr; }
    TaskHandle_t getDiagnosticsTaskHandle() const { return _diagnosticsTask; }
    TaskHandle_t getDHTTaskHandle() const { return _dhtTask; }
    TaskHandle_t getWebUITaskHandle() const { return _webUiTask; }
    bool isWebUITaskRunning() const { return _webUiTask != nullptr; }
    
private:
    TaskManager();
    ~TaskManager();
    TaskManager(const TaskManager&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;
    
    static void energyTaskFunc(void* param);
    static void accumulatorTaskFunc(void* param);
    static void modbusTaskFunc(void* param);
    static void tcpServerTaskFunc(void* param);
    static void mqttTaskFunc(void* param);
    static void diagnosticsTaskFunc(void* param);
    static void dhtTaskFunc(void* param);
    static void webUiTaskFunc(void* param);
    
    TaskHandle_t _energyTask;
    TaskHandle_t _accumulatorTask;
    TaskHandle_t _modbusTask;
    TaskHandle_t _tcpServerTask;
    TaskHandle_t _mqttTask;
    TaskHandle_t _diagnosticsTask;
    TaskHandle_t _dhtTask;
    TaskHandle_t _webUiTask;
    
    bool _tasksRunning;
    // NOTE: Synchronous WebServer can block during SPIFFS file streaming; running it in a dedicated Core0 task
    // can starve IDLE0 and trip TWDT in AP mode. Keep disabled by default and service in Arduino loop().
    static constexpr bool ENABLE_WEBUI_TASK = false;

    
    // Stack sizes (bytes)
    static constexpr uint32_t ENERGY_STACK_SIZE = 4096;
    static constexpr uint32_t ACCUMULATOR_STACK_SIZE = 4096;
    static constexpr uint32_t MODBUS_STACK_SIZE = 4096;
    static constexpr uint32_t TCP_SERVER_STACK_SIZE = 4096;
    static constexpr uint32_t MQTT_STACK_SIZE = 4096;
    static constexpr uint32_t DIAGNOSTICS_STACK_SIZE = 3072;
    static constexpr uint32_t DHT_STACK_SIZE = 3072;
    static constexpr uint32_t WEBUI_STACK_SIZE = 4096;
    
    // Task priorities (higher = more important)
    static constexpr UBaseType_t ENERGY_PRIORITY = 5;
    static constexpr UBaseType_t ACCUMULATOR_PRIORITY = 4;
    static constexpr UBaseType_t MODBUS_PRIORITY = 3;
    static constexpr UBaseType_t TCP_SERVER_PRIORITY = 2;
    static constexpr UBaseType_t MQTT_PRIORITY = 2;
    static constexpr UBaseType_t DIAGNOSTICS_PRIORITY = 1;
    static constexpr UBaseType_t DHT_PRIORITY = 1;
    static constexpr UBaseType_t WEBUI_PRIORITY = 2;
    
    // Core affinity (ESP32 dual-core)
    static constexpr BaseType_t CORE_0 = 0;  // Communications
    static constexpr BaseType_t CORE_1 = 1;  // Energy & Modbus
};

#endif // TASKMANAGER_H