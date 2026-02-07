/**
 * @file EventBus.h
 * @brief Inter-task event/message system using FreeRTOS queues
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Provides thread-safe publish/subscribe event system for task communication
 */

#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "../../include/GlobalTypes.h"
#include "../../include/ErrorCodes.h"

/**
 * @enum EventType
 * @brief Types of events that can be published on the event bus
 */
enum class EventType {
    // Data events
    DATA_READY,                 ///< New meter data available
    ENERGY_UPDATED,             ///< Energy accumulators updated
    CALIBRATION_CHANGED,        ///< Calibration parameters changed
    
    // Network events
    WIFI_CONNECTED,             ///< WiFi connection established
    WIFI_DISCONNECTED,          ///< WiFi connection lost
    ETHERNET_LINK_UP,           ///< Ethernet link up
    ETHERNET_LINK_DOWN,         ///< Ethernet link down
    NETWORK_CONNECTED,          ///< Any network connection established
    NETWORK_DISCONNECTED,       ///< All network connections lost
    
    // Configuration events
    CONFIG_CHANGED,             ///< Configuration updated
    CONFIG_RESET,               ///< Configuration reset to defaults
    
    // System events
    SYSTEM_BOOT,                ///< System boot complete
    SYSTEM_SHUTDOWN,            ///< System shutting down
    SYSTEM_ERROR,               ///< System error occurred
    SYSTEM_WARNING,             ///< System warning
    
    // Power quality events
    VOLTAGE_SAG,                ///< Voltage sag detected
    VOLTAGE_SWELL,              ///< Voltage swell detected
    FREQUENCY_DEVIATION,        ///< Frequency out of range
    THD_ALARM,                  ///< THD exceeds threshold
    PHASE_IMBALANCE,            ///< Phase imbalance detected
    
    // Storage events
    DATA_LOG_FULL,              ///< Data log storage full
    DATA_LOG_ERROR,             ///< Data logging error
    NVS_WRITE_ERROR,            ///< NVS write error
    SPIFFS_ERROR,               ///< SPIFFS error
    
    // Communication events
    MODBUS_REQUEST,             ///< Modbus request received
    MQTT_CONNECTED,             ///< MQTT broker connected
    MQTT_DISCONNECTED,          ///< MQTT broker disconnected
    WEB_CLIENT_CONNECTED,       ///< Web client connected
    WEB_CLIENT_DISCONNECTED,    ///< Web client disconnected
    OTA_START,                  ///< OTA update starting
    OTA_COMPLETE,               ///< OTA update complete
    OTA_ERROR,                  ///< OTA update error
    
    // User interface events
    BUTTON_SHORT_PRESS,         ///< Button short press
    BUTTON_LONG_PRESS,          ///< Button long press
    DISPLAY_PAGE_CHANGE,        ///< Display page changed
    
    // Unknown
    UNKNOWN_EVENT
};

/**
 * @struct Event
 * @brief Event structure containing type and optional data
 */
struct Event {
    EventType type;             ///< Event type
    uint32_t data1;             ///< Optional data field 1
    uint32_t data2;             ///< Optional data field 2
    void* pData;                ///< Optional pointer to data
    unsigned long timestamp;    ///< Event timestamp (millis)
    
    Event() : type(EventType::UNKNOWN_EVENT), data1(0), data2(0), 
              pData(nullptr), timestamp(0) {}
    
    Event(EventType t, uint32_t d1 = 0, uint32_t d2 = 0, void* p = nullptr) 
        : type(t), data1(d1), data2(d2), pData(p), timestamp(millis()) {}
};

/**
 * @typedef EventCallback
 * @brief Callback function type for event handlers
 */
typedef void (*EventCallback)(const Event& event);

/**
 * @class EventBus
 * @brief Thread-safe publish/subscribe event system using FreeRTOS queues
 * 
 * Singleton class providing:
 * - Thread-safe event publishing
 * - Subscriber registration with callbacks
 * - Multiple subscribers per event type
 * - Non-blocking event dispatch
 */
class EventBus {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to EventBus instance
     */
    static EventBus& getInstance();
    
    /**
     * @brief Initialize event bus
     * @param queueSize Size of event queue (default 20)
     * @return true if initialization successful
     */
    bool init(uint8_t queueSize = 20);
    
    /**
     * @brief Subscribe to an event type
     * @param type Event type to subscribe to
     * @param callback Callback function to invoke when event occurs
     * @return true if subscription successful
     * 
     * Multiple subscribers can register for the same event type.
     * Callbacks are invoked in the order they were registered.
     */
    bool subscribe(EventType type, EventCallback callback);
    
    /**
     * @brief Unsubscribe from an event type
     * @param type Event type to unsubscribe from
     * @param callback Callback function to remove
     * @return true if unsubscribed successfully
     */
    bool unsubscribe(EventType type, EventCallback callback);
    
    /**
     * @brief Publish an event to the bus
     * @param event Event to publish
     * @return true if event queued successfully
     * 
     * Events are queued and dispatched in the next process() call.
     * Non-blocking operation.
     */
    bool publish(const Event& event);
    
    /**
     * @brief Publish an event with simple parameters
     * @param type Event type
     * @param data1 Optional data field 1
     * @param data2 Optional data field 2
     * @param pData Optional pointer to data
     * @return true if event queued successfully
     */
    bool publish(EventType type, uint32_t data1 = 0, uint32_t data2 = 0, void* pData = nullptr);
    
    /**
     * @brief Process queued events and dispatch to subscribers
     * 
     * Should be called regularly from main loop or a dedicated task.
     * Processes all queued events and invokes registered callbacks.
     */
    void process();
    
    /**
     * @brief Get number of queued events
     * @return Number of events waiting to be processed
     */
    uint8_t getQueuedEventCount() const;
    
    /**
     * @brief Get number of subscribers for an event type
     * @param type Event type
     * @return Number of subscribers
     */
    uint8_t getSubscriberCount(EventType type) const;
    
    /**
     * @brief Clear all events from queue
     */
    void clearQueue();
    
    /**
     * @brief Get event type as string
     * @param type Event type
     * @return Event type name
     */
    static const char* eventTypeToString(EventType type);

private:
    EventBus() : eventQueue(nullptr), initialized(false) {}
    ~EventBus();
    
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    
    /**
     * @struct Subscriber
     * @brief Internal subscriber information
     */
    struct Subscriber {
        EventType type;
        EventCallback callback;
        Subscriber* next;
        
        Subscriber(EventType t, EventCallback cb) 
            : type(t), callback(cb), next(nullptr) {}
    };
    
    QueueHandle_t eventQueue;       ///< FreeRTOS event queue
    Subscriber* subscribers;        ///< Linked list of subscribers
    bool initialized;               ///< Initialization status
    mutable SemaphoreHandle_t mutex; ///< Mutex for thread safety
    
    /**
     * @brief Dispatch event to all subscribers
     * @param event Event to dispatch
     */
    void dispatchEvent(const Event& event);
};
