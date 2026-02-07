/**
 * @file EventBus.cpp
 * @brief Implementation of EventBus
 */

#include "EventBus.h"
#include "../diagnostics/Logger.h"

EventBus& EventBus::getInstance() {
    static EventBus instance;
    return instance;
}

bool EventBus::init(uint8_t queueSize) {
    if (initialized) {
        return true;
    }
    
    // Create FreeRTOS queue for events
    eventQueue = xQueueCreate(queueSize, sizeof(Event));
    if (eventQueue == nullptr) {
        LOG_ERROR("EventBus", "Failed to create event queue");
        return false;
    }
    
    // Create mutex for thread safety
    mutex = xSemaphoreCreateMutex();
    if (mutex == nullptr) {
        LOG_ERROR("EventBus", "Failed to create mutex");
        vQueueDelete(eventQueue);
        eventQueue = nullptr;
        return false;
    }
    
    subscribers = nullptr;
    initialized = true;
    
    LOG_INFO("EventBus", "Initialized with queue size: %d", queueSize);
    return true;
}

bool EventBus::subscribe(EventType type, EventCallback callback) {
    if (!initialized || callback == nullptr) {
        return false;
    }
    
    // Lock mutex
    if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        LOG_ERROR("EventBus", "Failed to acquire mutex for subscribe");
        return false;
    }
    
    // Check if already subscribed
    Subscriber* current = subscribers;
    while (current != nullptr) {
        if (current->type == type && current->callback == callback) {
            xSemaphoreGive(mutex);
            return true; // Already subscribed
        }
        current = current->next;
    }
    
    // Create new subscriber
    Subscriber* newSub = new Subscriber(type, callback);
    if (newSub == nullptr) {
        LOG_ERROR("EventBus", "Failed to allocate subscriber");
        xSemaphoreGive(mutex);
        return false;
    }
    
    // Add to head of linked list
    newSub->next = subscribers;
    subscribers = newSub;
    
    xSemaphoreGive(mutex);
    
    LOG_DEBUG("EventBus", "Subscribed to event: %s", eventTypeToString(type));
    return true;
}

bool EventBus::unsubscribe(EventType type, EventCallback callback) {
    if (!initialized || callback == nullptr) {
        return false;
    }
    
    // Lock mutex
    if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        LOG_ERROR("EventBus", "Failed to acquire mutex for unsubscribe");
        return false;
    }
    
    // Find and remove subscriber
    Subscriber* current = subscribers;
    Subscriber* prev = nullptr;
    
    while (current != nullptr) {
        if (current->type == type && current->callback == callback) {
            // Found it, remove from list
            if (prev == nullptr) {
                subscribers = current->next;
            } else {
                prev->next = current->next;
            }
            
            delete current;
            xSemaphoreGive(mutex);
            
            LOG_DEBUG("EventBus", "Unsubscribed from event: %s", eventTypeToString(type));
            return true;
        }
        
        prev = current;
        current = current->next;
    }
    
    xSemaphoreGive(mutex);
    return false; // Not found
}

bool EventBus::publish(const Event& event) {
    if (!initialized) {
        return false;
    }
    
    // Try to queue the event (non-blocking)
    if (xQueueSend(eventQueue, &event, 0) != pdTRUE) {
        LOG_WARN("EventBus", "Event queue full, dropping event: %s", 
                 eventTypeToString(event.type));
        return false;
    }
    
    return true;
}

bool EventBus::publish(EventType type, uint32_t data1, uint32_t data2, void* pData) {
    Event event(type, data1, data2, pData);
    return publish(event);
}

void EventBus::process() {
    if (!initialized) {
        return;
    }
    
    Event event;
    
    // Process all queued events
    while (xQueueReceive(eventQueue, &event, 0) == pdTRUE) {
        dispatchEvent(event);
    }
}

void EventBus::dispatchEvent(const Event& event) {
    // Lock mutex
    if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        LOG_ERROR("EventBus", "Failed to acquire mutex for dispatch");
        return;
    }
    
    // Invoke all callbacks for this event type
    Subscriber* current = subscribers;
    int count = 0;
    
    while (current != nullptr) {
        if (current->type == event.type) {
            // Invoke callback (unlocking mutex during callback to prevent deadlock)
            xSemaphoreGive(mutex);
            
            if (current->callback != nullptr) {
                current->callback(event);
                count++;
            }
            
            // Re-acquire mutex
            if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
                LOG_ERROR("EventBus", "Failed to re-acquire mutex after callback");
                return;
            }
        }
        current = current->next;
    }
    
    xSemaphoreGive(mutex);
    
    if (count > 0) {
        LOG_DEBUG("EventBus", "Dispatched event %s to %d subscriber(s)", 
                  eventTypeToString(event.type), count);
    }
}

uint8_t EventBus::getQueuedEventCount() const {
    if (!initialized || eventQueue == nullptr) {
        return 0;
    }
    return uxQueueMessagesWaiting(eventQueue);
}

uint8_t EventBus::getSubscriberCount(EventType type) const {
    if (!initialized) {
        return 0;
    }
    
    // Lock mutex
    if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return 0;
    }
    
    uint8_t count = 0;
    Subscriber* current = subscribers;
    
    while (current != nullptr) {
        if (current->type == type) {
            count++;
        }
        current = current->next;
    }
    
    xSemaphoreGive(mutex);
    return count;
}

void EventBus::clearQueue() {
    if (!initialized || eventQueue == nullptr) {
        return;
    }
    
    xQueueReset(eventQueue);
    LOG_INFO("EventBus", "Event queue cleared");
}

const char* EventBus::eventTypeToString(EventType type) {
    switch (type) {
        case EventType::DATA_READY: return "DATA_READY";
        case EventType::ENERGY_UPDATED: return "ENERGY_UPDATED";
        case EventType::CALIBRATION_CHANGED: return "CALIBRATION_CHANGED";
        
        case EventType::WIFI_CONNECTED: return "WIFI_CONNECTED";
        case EventType::WIFI_DISCONNECTED: return "WIFI_DISCONNECTED";
        case EventType::ETHERNET_LINK_UP: return "ETHERNET_LINK_UP";
        case EventType::ETHERNET_LINK_DOWN: return "ETHERNET_LINK_DOWN";
        case EventType::NETWORK_CONNECTED: return "NETWORK_CONNECTED";
        case EventType::NETWORK_DISCONNECTED: return "NETWORK_DISCONNECTED";
        
        case EventType::CONFIG_CHANGED: return "CONFIG_CHANGED";
        case EventType::CONFIG_RESET: return "CONFIG_RESET";
        
        case EventType::SYSTEM_BOOT: return "SYSTEM_BOOT";
        case EventType::SYSTEM_SHUTDOWN: return "SYSTEM_SHUTDOWN";
        case EventType::SYSTEM_ERROR: return "SYSTEM_ERROR";
        case EventType::SYSTEM_WARNING: return "SYSTEM_WARNING";
        
        case EventType::VOLTAGE_SAG: return "VOLTAGE_SAG";
        case EventType::VOLTAGE_SWELL: return "VOLTAGE_SWELL";
        case EventType::FREQUENCY_DEVIATION: return "FREQUENCY_DEVIATION";
        case EventType::THD_ALARM: return "THD_ALARM";
        case EventType::PHASE_IMBALANCE: return "PHASE_IMBALANCE";
        
        case EventType::DATA_LOG_FULL: return "DATA_LOG_FULL";
        case EventType::DATA_LOG_ERROR: return "DATA_LOG_ERROR";
        case EventType::NVS_WRITE_ERROR: return "NVS_WRITE_ERROR";
        case EventType::SPIFFS_ERROR: return "SPIFFS_ERROR";
        
        case EventType::MODBUS_REQUEST: return "MODBUS_REQUEST";
        case EventType::MQTT_CONNECTED: return "MQTT_CONNECTED";
        case EventType::MQTT_DISCONNECTED: return "MQTT_DISCONNECTED";
        case EventType::WEB_CLIENT_CONNECTED: return "WEB_CLIENT_CONNECTED";
        case EventType::WEB_CLIENT_DISCONNECTED: return "WEB_CLIENT_DISCONNECTED";
        case EventType::OTA_START: return "OTA_START";
        case EventType::OTA_COMPLETE: return "OTA_COMPLETE";
        case EventType::OTA_ERROR: return "OTA_ERROR";
        
        case EventType::BUTTON_SHORT_PRESS: return "BUTTON_SHORT_PRESS";
        case EventType::BUTTON_LONG_PRESS: return "BUTTON_LONG_PRESS";
        case EventType::DISPLAY_PAGE_CHANGE: return "DISPLAY_PAGE_CHANGE";
        
        default: return "UNKNOWN_EVENT";
    }
}

EventBus::~EventBus() {
    if (initialized) {
        // Clean up subscribers
        if (xSemaphoreTake(mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
            Subscriber* current = subscribers;
            while (current != nullptr) {
                Subscriber* next = current->next;
                delete current;
                current = next;
            }
            subscribers = nullptr;
            xSemaphoreGive(mutex);
        }
        
        // Delete queue and mutex
        if (eventQueue != nullptr) {
            vQueueDelete(eventQueue);
        }
        if (mutex != nullptr) {
            vSemaphoreDelete(mutex);
        }
    }
}
