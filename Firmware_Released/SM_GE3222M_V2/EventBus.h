/**
 * @file EventBus.h
 * @brief Thread-safe event system for inter-module communication
 * 
 * Singleton pub/sub event bus using FreeRTOS queues for event delivery.
 * Supports up to 10 subscribers per event type with thread-safe access.
 */

#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>
#include "DataTypes.h"

// Event callback signature
typedef void (*EventCallback)(EventType type, void* data);

class EventBus {
public:
    static EventBus& getInstance();
    
    bool init();
    
    bool subscribe(EventType type, EventCallback callback);
    bool unsubscribe(EventType type, EventCallback callback);
    bool publish(EventType type, void* data = nullptr, size_t dataSize = 0);
    
    void handle();
    
private:
    EventBus();
    ~EventBus();
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    
    struct EventMessage {
        EventType type;
        uint8_t data[256];
        size_t dataSize;
        
        EventMessage() : type(EventType::METER_DATA_UPDATED), dataSize(0) {
            memset(data, 0, sizeof(data));
        }
    };
    
    struct Subscriber {
        EventCallback callback;
        bool active;
        
        Subscriber() : callback(nullptr), active(false) {}
    };
    
    static constexpr uint8_t MAX_SUBSCRIBERS_PER_EVENT = 10;
    static constexpr uint8_t MAX_EVENT_TYPES = 11;
    static constexpr uint8_t EVENT_QUEUE_SIZE = 20;
    
    Subscriber _subscribers[MAX_EVENT_TYPES][MAX_SUBSCRIBERS_PER_EVENT];
    QueueHandle_t _eventQueue;
    SemaphoreHandle_t _mutex;
    bool _initialized;
    
    uint8_t eventTypeToIndex(EventType type) const;
    void processEvent(const EventMessage& msg);
};

#endif // EVENTBUS_H
