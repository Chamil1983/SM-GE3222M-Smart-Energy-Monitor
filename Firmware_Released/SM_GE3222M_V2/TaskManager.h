/**
 * @file TaskManager.h
 * @brief FreeRTOS task management for SM-GE3222M V2.0
 * 
 * Singleton pattern managing all 7 system tasks with proper core affinity,
 * priorities, and stack sizes for ESP32 dual-core architecture.
 * 
 * Task Architecture:
 * Core 1 (Energy & Modbus):
 *   - EnergyTask: Read ATM90E36, update meter (500ms, P5)
 *   - AccumulatorTask: Update energy accumulator, auto-save (1000ms, P4)
 *   - ModbusTask: Handle Modbus RTU/TCP (10ms poll, P3)
 * 
 * Core 0 (Communications & Diagnostics):
 *   - TCPServerTask: Handle TCP data server (event-driven, P3)
 *   - WebServerTask: Handle WebSocket broadcasts (10ms, P2)
 *   - MQTTTask: Handle MQTT publishing (configurable, P2)
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
    TaskHandle_t getWebServerTaskHandle() const { return _webServerTask; }
    TaskHandle_t getMQTTTaskHandle() const { return _mqttTask; }
    TaskHandle_t getDiagnosticsTaskHandle() const { return _diagnosticsTask; }
    
private:
    TaskManager();
    ~TaskManager();
    TaskManager(const TaskManager&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;
    
    static void energyTaskFunc(void* param);
    static void accumulatorTaskFunc(void* param);
    static void modbusTaskFunc(void* param);
    static void tcpServerTaskFunc(void* param);
    static void webServerTaskFunc(void* param);
    static void mqttTaskFunc(void* param);
    static void diagnosticsTaskFunc(void* param);
    
    TaskHandle_t _energyTask;
    TaskHandle_t _accumulatorTask;
    TaskHandle_t _modbusTask;
    TaskHandle_t _tcpServerTask;
    TaskHandle_t _webServerTask;
    TaskHandle_t _mqttTask;
    TaskHandle_t _diagnosticsTask;
    
    bool _tasksRunning;
    
    // Stack sizes (bytes)
    static constexpr uint32_t ENERGY_STACK_SIZE = 4096;
    static constexpr uint32_t ACCUMULATOR_STACK_SIZE = 4096;
    static constexpr uint32_t MODBUS_STACK_SIZE = 4096;
    static constexpr uint32_t TCP_STACK_SIZE = 8192;
    static constexpr uint32_t WEB_STACK_SIZE = 8192;
    static constexpr uint32_t MQTT_STACK_SIZE = 4096;
    static constexpr uint32_t DIAGNOSTICS_STACK_SIZE = 3072;
    
    // Task priorities (higher = more important)
    static constexpr UBaseType_t ENERGY_PRIORITY = 5;
    static constexpr UBaseType_t ACCUMULATOR_PRIORITY = 4;
    static constexpr UBaseType_t MODBUS_PRIORITY = 3;
    static constexpr UBaseType_t TCP_PRIORITY = 3;
    static constexpr UBaseType_t WEB_PRIORITY = 2;
    static constexpr UBaseType_t MQTT_PRIORITY = 2;
    static constexpr UBaseType_t DIAGNOSTICS_PRIORITY = 1;
    
    // Core affinity (ESP32 dual-core)
    static constexpr BaseType_t CORE_0 = 0;  // Communications
    static constexpr BaseType_t CORE_1 = 1;  // Energy & Modbus
};

#endif // TASKMANAGER_H
