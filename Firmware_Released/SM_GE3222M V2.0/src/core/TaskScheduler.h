/**
 * @file TaskScheduler.h
 * @brief Non-blocking cooperative task scheduler with priorities
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Provides a non-blocking task scheduling system for managing multiple
 * concurrent tasks with different priorities and execution intervals.
 */

#pragma once

#include <Arduino.h>
#include "../../include/GlobalTypes.h"

#define MAX_TASKS 32

/**
 * @class TaskScheduler
 * @brief Manages non-blocking cooperative task execution with priorities
 * 
 * This scheduler executes tasks based on their interval and priority without
 * using blocking delay() calls. Tasks are executed in priority order when due.
 */
class TaskScheduler {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to TaskScheduler instance
     */
    static TaskScheduler& getInstance();
    
    /**
     * @brief Add a task to the scheduler
     * @param name Task name (must be unique)
     * @param callback Function to execute
     * @param intervalMs Execution interval in milliseconds
     * @param priority Task priority (CRITICAL to IDLE)
     * @return true if task added successfully, false if scheduler is full
     */
    bool addTask(const char* name, void (*callback)(), unsigned long intervalMs, 
                 TaskPriority priority = TaskPriority::PRIORITY_MEDIUM);
    
    /**
     * @brief Remove a task from the scheduler
     * @param name Task name to remove
     * @return true if task removed successfully, false if not found
     */
    bool removeTask(const char* name);
    
    /**
     * @brief Enable a task
     * @param name Task name to enable
     * @return true if task enabled successfully, false if not found
     */
    bool enableTask(const char* name);
    
    /**
     * @brief Disable a task
     * @param name Task name to disable
     * @return true if task disabled successfully, false if not found
     */
    bool disableTask(const char* name);
    
    /**
     * @brief Execute all due tasks in priority order
     * 
     * This should be called repeatedly in the main loop.
     * Tasks are executed from HIGH to LOW priority.
     */
    void run();
    
    /**
     * @brief Get task execution time
     * @param name Task name
     * @return Last execution time in microseconds, 0 if not found
     */
    unsigned long getTaskExecutionTime(const char* name) const;
    
    /**
     * @brief Get number of tasks
     * @return Current number of tasks in scheduler
     */
    uint8_t getTaskCount() const { return taskCount; }
    
    /**
     * @brief Clear all tasks
     */
    void clear();
    
    /**
     * @brief Check if a task exists
     * @param name Task name
     * @return true if task exists, false otherwise
     */
    bool hasTask(const char* name) const;

private:
    TaskScheduler() : taskCount(0) {}
    TaskScheduler(const TaskScheduler&) = delete;
    TaskScheduler& operator=(const TaskScheduler&) = delete;
    
    Task tasks[MAX_TASKS];
    uint8_t taskCount;
    
    /**
     * @brief Find task index by name
     * @param name Task name to find
     * @return Task index, or -1 if not found
     */
    int findTaskIndex(const char* name) const;
    
    /**
     * @brief Check if task is due for execution
     * @param task Task to check
     * @return true if task should run now
     */
    bool isTaskDue(const Task& task) const;
};
