/**
 * @file TaskScheduler.cpp
 * @brief Implementation of non-blocking cooperative task scheduler
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "TaskScheduler.h"

TaskScheduler& TaskScheduler::getInstance() {
    static TaskScheduler instance;
    return instance;
}

bool TaskScheduler::addTask(const char* name, void (*callback)(), 
                            unsigned long intervalMs, TaskPriority priority) {
    if (taskCount >= MAX_TASKS || callback == nullptr || name == nullptr) {
        return false;
    }
    
    // Check if task already exists
    if (hasTask(name)) {
        return false;
    }
    
    // Add new task
    tasks[taskCount].name = name;
    tasks[taskCount].callback = callback;
    tasks[taskCount].interval = intervalMs;
    tasks[taskCount].lastRun = 0;
    tasks[taskCount].priority = priority;
    tasks[taskCount].enabled = true;
    tasks[taskCount].executionTime = 0;
    
    taskCount++;
    return true;
}

bool TaskScheduler::removeTask(const char* name) {
    int index = findTaskIndex(name);
    if (index < 0) {
        return false;
    }
    
    // Shift remaining tasks down
    for (uint8_t i = index; i < taskCount - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    
    taskCount--;
    return true;
}

bool TaskScheduler::enableTask(const char* name) {
    int index = findTaskIndex(name);
    if (index < 0) {
        return false;
    }
    
    tasks[index].enabled = true;
    return true;
}

bool TaskScheduler::disableTask(const char* name) {
    int index = findTaskIndex(name);
    if (index < 0) {
        return false;
    }
    
    tasks[index].enabled = false;
    return true;
}

void TaskScheduler::run() {
    unsigned long currentMillis = millis();
    
    // Execute tasks in priority order (CRITICAL to IDLE)
    for (uint8_t priority = 0; priority <= 4; priority++) {
        for (uint8_t i = 0; i < taskCount; i++) {
            Task& task = tasks[i];
            
            // Skip if wrong priority, disabled, or not due
            if (static_cast<uint8_t>(task.priority) != priority) {
                continue;
            }
            
            if (!task.enabled) {
                continue;
            }
            
            if (!isTaskDue(task)) {
                continue;
            }
            
            // Execute task and measure execution time
            unsigned long startMicros = micros();
            task.callback();
            unsigned long endMicros = micros();
            
            task.executionTime = endMicros - startMicros;
            task.lastRun = currentMillis;
        }
    }
}

unsigned long TaskScheduler::getTaskExecutionTime(const char* name) const {
    int index = findTaskIndex(name);
    if (index < 0) {
        return 0;
    }
    
    return tasks[index].executionTime;
}

void TaskScheduler::clear() {
    taskCount = 0;
}

bool TaskScheduler::hasTask(const char* name) const {
    return findTaskIndex(name) >= 0;
}

int TaskScheduler::findTaskIndex(const char* name) const {
    if (name == nullptr) {
        return -1;
    }
    
    for (uint8_t i = 0; i < taskCount; i++) {
        if (strcmp(tasks[i].name, name) == 0) {
            return i;
        }
    }
    
    return -1;
}

bool TaskScheduler::isTaskDue(const Task& task) const {
    unsigned long currentMillis = millis();
    
    // Handle first run
    if (task.lastRun == 0) {
        return true;
    }
    
    // Check if interval has elapsed (handles millis() overflow)
    return (currentMillis - task.lastRun) >= task.interval;
}
