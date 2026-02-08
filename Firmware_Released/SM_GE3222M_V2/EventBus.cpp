/**
 * @file EventBus.cpp
 * @brief Implementation of thread-safe event system
 */

#include "EventBus.h"
#include "Logger.h"

EventBus& EventBus::getInstance() {
    static EventBus instance;
    return instance;
}

EventBus::EventBus() 
    : _eventQueue(nullptr)
    , _mutex(nullptr)
    , _initialized(false) {
}

EventBus::~EventBus() {
    if (_eventQueue) {
        vQueueDelete(_eventQueue);
    }
    if (_mutex) {
        vSemaphoreDelete(_mutex);
    }
}

bool EventBus::init() {
    if (_initialized) {
        return true;
    }
    
    _mutex = xSemaphoreCreateMutex();
    if (!_mutex) {
        Logger::getInstance().error("EventBus: Failed to create mutex");
        return false;
    }
    
    _eventQueue = xQueueCreate(EVENT_QUEUE_SIZE, sizeof(EventMessage));
    if (!_eventQueue) {
        Logger::getInstance().error("EventBus: Failed to create event queue");
        vSemaphoreDelete(_mutex);
        _mutex = nullptr;
        return false;
    }
    
    for (uint8_t i = 0; i < MAX_EVENT_TYPES; i++) {
        for (uint8_t j = 0; j < MAX_SUBSCRIBERS_PER_EVENT; j++) {
            _subscribers[i][j].active = false;
            _subscribers[i][j].callback = nullptr;
        }
    }
    
    _initialized = true;
    Logger::getInstance().info("EventBus: Initialized");
    return true;
}

bool EventBus::subscribe(EventType type, EventCallback callback) {
    if (!_initialized || !callback) {
        return false;
    }
    
    uint8_t idx = eventTypeToIndex(type);
    if (idx >= MAX_EVENT_TYPES) {
        return false;
    }
    
    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Logger::getInstance().warn("EventBus: Subscribe mutex timeout");
        return false;
    }
    
    bool subscribed = false;
    for (uint8_t i = 0; i < MAX_SUBSCRIBERS_PER_EVENT; i++) {
        if (!_subscribers[idx][i].active) {
            _subscribers[idx][i].callback = callback;
            _subscribers[idx][i].active = true;
            subscribed = true;
            Logger::getInstance().debug("EventBus: Subscribed to event %d", (int)type);
            break;
        }
    }
    
    xSemaphoreGive(_mutex);
    
    if (!subscribed) {
        Logger::getInstance().warn("EventBus: Max subscribers reached for event %d", (int)type);
    }
    
    return subscribed;
}

bool EventBus::unsubscribe(EventType type, EventCallback callback) {
    if (!_initialized || !callback) {
        return false;
    }
    
    uint8_t idx = eventTypeToIndex(type);
    if (idx >= MAX_EVENT_TYPES) {
        return false;
    }
    
    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Logger::getInstance().warn("EventBus: Unsubscribe mutex timeout");
        return false;
    }
    
    bool unsubscribed = false;
    for (uint8_t i = 0; i < MAX_SUBSCRIBERS_PER_EVENT; i++) {
        if (_subscribers[idx][i].active && _subscribers[idx][i].callback == callback) {
            _subscribers[idx][i].active = false;
            _subscribers[idx][i].callback = nullptr;
            unsubscribed = true;
            Logger::getInstance().debug("EventBus: Unsubscribed from event %d", (int)type);
            break;
        }
    }
    
    xSemaphoreGive(_mutex);
    return unsubscribed;
}

bool EventBus::publish(EventType type, void* data, size_t dataSize) {
    if (!_initialized) {
        return false;
    }
    
    if (dataSize > sizeof(EventMessage::data)) {
        Logger::getInstance().warn("EventBus: Event data too large (%d bytes)", dataSize);
        return false;
    }
    
    EventMessage msg;
    msg.type = type;
    msg.dataSize = dataSize;
    
    if (data && dataSize > 0) {
        memcpy(msg.data, data, dataSize);
    }
    
    if (xQueueSend(_eventQueue, &msg, pdMS_TO_TICKS(10)) != pdTRUE) {
        Logger::getInstance().warn("EventBus: Event queue full, dropping event %d", (int)type);
        return false;
    }
    
    return true;
}

void EventBus::handle() {
    if (!_initialized) {
        return;
    }
    
    EventMessage msg;
    while (xQueueReceive(_eventQueue, &msg, 0) == pdTRUE) {
        processEvent(msg);
    }
}

void EventBus::processEvent(const EventMessage& msg) {
    uint8_t idx = eventTypeToIndex(msg.type);
    if (idx >= MAX_EVENT_TYPES) {
        return;
    }
    
    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Logger::getInstance().warn("EventBus: Process mutex timeout");
        return;
    }
    
    for (uint8_t i = 0; i < MAX_SUBSCRIBERS_PER_EVENT; i++) {
        if (_subscribers[idx][i].active && _subscribers[idx][i].callback) {
            void* dataPtr = (msg.dataSize > 0) ? (void*)msg.data : nullptr;
            _subscribers[idx][i].callback(msg.type, dataPtr);
        }
    }
    
    xSemaphoreGive(_mutex);
}

uint8_t EventBus::eventTypeToIndex(EventType type) const {
    return static_cast<uint8_t>(type);
}
