/**
 * @file RingBuffer.h
 * @brief Template-based thread-safe circular buffer
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Generic circular buffer for historical data storage
 */

#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

/**
 * @class RingBuffer
 * @brief Thread-safe circular buffer template
 * 
 * Template class providing:
 * - Fixed-size circular buffer
 * - Thread-safe push/pop operations
 * - Overwrite oldest data when full
 * - Iterator-style access
 */
template<typename T, size_t SIZE>
class RingBuffer {
public:
    /**
     * @brief Constructor
     */
    RingBuffer() : head(0), tail(0), count(0), full(false) {
        mutex = xSemaphoreCreateMutex();
    }
    
    /**
     * @brief Destructor
     */
    ~RingBuffer() {
        if (mutex != nullptr) {
            vSemaphoreDelete(mutex);
        }
    }
    
    /**
     * @brief Push item to buffer
     * @param item Item to push
     * @return true if pushed successfully
     * 
     * If buffer is full, overwrites oldest item
     */
    bool push(const T& item) {
        if (mutex == nullptr) {
            return false;
        }
        
        if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            return false;
        }
        
        buffer[head] = item;
        head = (head + 1) % SIZE;
        
        if (full) {
            tail = (tail + 1) % SIZE;  // Overwrite oldest
        }
        
        if (head == tail) {
            full = true;
        }
        
        if (count < SIZE) {
            count++;
        }
        
        xSemaphoreGive(mutex);
        return true;
    }
    
    /**
     * @brief Pop item from buffer (oldest first - FIFO)
     * @param item Reference to store popped item
     * @return true if popped successfully, false if empty
     */
    bool pop(T& item) {
        if (mutex == nullptr) {
            return false;
        }
        
        if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            return false;
        }
        
        if (count == 0) {
            xSemaphoreGive(mutex);
            return false;
        }
        
        item = buffer[tail];
        tail = (tail + 1) % SIZE;
        full = false;
        count--;
        
        xSemaphoreGive(mutex);
        return true;
    }
    
    /**
     * @brief Peek at item without removing (FIFO order)
     * @param index Index from tail (0 = oldest, count-1 = newest)
     * @param item Reference to store peeked item
     * @return true if successful
     */
    bool peek(size_t index, T& item) const {
        if (mutex == nullptr) {
            return false;
        }
        
        if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            return false;
        }
        
        if (index >= count) {
            xSemaphoreGive(mutex);
            return false;
        }
        
        size_t actualIndex = (tail + index) % SIZE;
        item = buffer[actualIndex];
        
        xSemaphoreGive(mutex);
        return true;
    }
    
    /**
     * @brief Get newest item (most recently pushed)
     * @param item Reference to store item
     * @return true if successful
     */
    bool getNewest(T& item) const {
        if (count == 0) {
            return false;
        }
        return peek(count - 1, item);
    }
    
    /**
     * @brief Get oldest item (first to be popped)
     * @param item Reference to store item
     * @return true if successful
     */
    bool getOldest(T& item) const {
        return peek(0, item);
    }
    
    /**
     * @brief Clear all items from buffer
     */
    void clear() {
        if (mutex == nullptr) {
            return;
        }
        
        if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            head = 0;
            tail = 0;
            count = 0;
            full = false;
            xSemaphoreGive(mutex);
        }
    }
    
    /**
     * @brief Get number of items in buffer
     * @return Item count
     */
    size_t size() const {
        return count;
    }
    
    /**
     * @brief Get maximum capacity
     * @return Buffer capacity
     */
    size_t capacity() const {
        return SIZE;
    }
    
    /**
     * @brief Check if buffer is empty
     * @return true if empty
     */
    bool isEmpty() const {
        return count == 0;
    }
    
    /**
     * @brief Check if buffer is full
     * @return true if full
     */
    bool isFull() const {
        return full;
    }
    
    /**
     * @brief Get fill percentage
     * @return Percentage (0-100)
     */
    float getFillPercent() const {
        return (float)count / (float)SIZE * 100.0;
    }

private:
    T buffer[SIZE];                 ///< Internal buffer array
    size_t head;                    ///< Write index
    size_t tail;                    ///< Read index
    size_t count;                   ///< Current count
    bool full;                      ///< Full flag
    mutable SemaphoreHandle_t mutex; ///< Mutex for thread safety
};
