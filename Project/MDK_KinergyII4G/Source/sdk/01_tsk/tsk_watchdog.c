/**
 * =====================================================================================
 * @file        tsk_watchdog.c
 * @brief       系统任务看门狗实现
 * @author      23Elapse
 * @version     1.0
 * @date        2025-09-08
 * @note        监控关键任务状态，防止系统死锁
 * =====================================================================================
 */

#include "tsk_watchdog.h"
#include "bsp_log_manager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* ========================== 私有变量定义 ========================== */
static TimerHandle_t s_watchdog_timer = NULL;
static TaskStatus_t s_task_status[10];
static UBaseType_t s_task_count = 0;
static bool s_watchdog_enabled = false;

/* ========================== 私有函数声明 ========================== */
static void watchdog_timer_callback(TimerHandle_t xTimer);
static void check_task_states(void);
static bool is_task_stuck(TaskStatus_t* task_status);

/* ========================== 公共函数实现 ========================== */

/**
 * @brief 初始化任务看门狗
 * @return bool 初始化是否成功
 */
bool TaskWatchdog_Init(void)
{
    if (s_watchdog_timer != NULL) {
        LOG_WARN(LOG_MODULE_SYSTEM, "Task watchdog already initialized");
        return true;
    }
    
    /* 创建看门狗定时器，每5秒检查一次 */
    s_watchdog_timer = xTimerCreate("TaskWatchdog",
                                   pdMS_TO_TICKS(5000),
                                   pdTRUE,  // 自动重载
                                   NULL,
                                   watchdog_timer_callback);
    
    if (s_watchdog_timer == NULL) {
        LOG_ERROR(LOG_MODULE_SYSTEM, "Failed to create watchdog timer");
        return false;
    }
    
    LOG_INFO(LOG_MODULE_SYSTEM, "Task watchdog initialized successfully");
    return true;
}

/**
 * @brief 启动任务看门狗
 * @return bool 启动是否成功
 */
bool TaskWatchdog_Start(void)
{
    if (s_watchdog_timer == NULL) {
        LOG_ERROR(LOG_MODULE_SYSTEM, "Watchdog not initialized");
        return false;
    }
    
    if (xTimerStart(s_watchdog_timer, pdMS_TO_TICKS(100)) != pdPASS) {
        LOG_ERROR(LOG_MODULE_SYSTEM, "Failed to start watchdog timer");
        return false;
    }
    
    s_watchdog_enabled = true;
    LOG_INFO(LOG_MODULE_SYSTEM, "Task watchdog started");
    return true;
}

/**
 * @brief 停止任务看门狗
 */
void TaskWatchdog_Stop(void)
{
    if (s_watchdog_timer != NULL) {
        xTimerStop(s_watchdog_timer, pdMS_TO_TICKS(100));
        s_watchdog_enabled = false;
        LOG_INFO(LOG_MODULE_SYSTEM, "Task watchdog stopped");
    }
}

/**
 * @brief 检查系统是否死锁
 * @return bool true-系统正常，false-可能死锁
 */
bool TaskWatchdog_CheckSystem(void)
{
    if (!s_watchdog_enabled) {
        return true;
    }
    
    check_task_states();
    return true;  // 简化实现，实际可以根据任务状态返回
}

/* ========================== 私有函数实现 ========================== */

/**
 * @brief 看门狗定时器回调函数
 * @param xTimer 定时器句柄
 */
static void watchdog_timer_callback(TimerHandle_t xTimer)
{
    (void)xTimer;  // 避免未使用警告
    
    LOG_DEBUG(LOG_MODULE_SYSTEM, "Watchdog timer triggered, checking system status");
    
    check_task_states();
}

/**
 * @brief 检查任务状态
 */
static void check_task_states(void)
{
    /* 获取所有任务状态 */
    s_task_count = uxTaskGetSystemState(s_task_status, sizeof(s_task_status)/sizeof(s_task_status[0]), NULL);
    
    LOG_DEBUG(LOG_MODULE_SYSTEM, "System has %d active tasks", s_task_count);
    
    /* 检查关键任务 */
    for (UBaseType_t i = 0; i < s_task_count; i++) {
        TaskStatus_t* task = &s_task_status[i];
        
        /* 检查4G相关任务 */
        if (strstr(task->pcTaskName, "4G") != NULL ||
            strstr(task->pcTaskName, "Commu") != NULL) {
            
            if (is_task_stuck(task)) {
                LOG_WARN(LOG_MODULE_SYSTEM, "Task %s may be stuck (state: %d, priority: %d)",
                        task->pcTaskName, task->eCurrentState, task->uxCurrentPriority);
            } else {
                LOG_DEBUG(LOG_MODULE_SYSTEM, "Task %s is healthy (state: %d)",
                         task->pcTaskName, task->eCurrentState);
            }
        }
    }
    
    /* 检查堆栈使用情况 */
    size_t free_heap = xPortGetFreeHeapSize();
    if (free_heap < 1024) {  // 小于1KB
        LOG_WARN(LOG_MODULE_SYSTEM, "Low heap memory: %d bytes free", free_heap);
    }
}

/**
 * @brief 判断任务是否卡死
 * @param task_status 任务状态
 * @return bool true-可能卡死，false-正常
 */
static bool is_task_stuck(TaskStatus_t* task_status)
{
    /* 简化实现：检查任务状态 */
    if (task_status->eCurrentState == eBlocked) {
        /* 任务被阻塞，这在等待信号量时是正常的 */
        return false;
    }
    
    if (task_status->eCurrentState == eSuspended) {
        LOG_WARN(LOG_MODULE_SYSTEM, "Task %s is suspended", task_status->pcTaskName);
        return true;
    }
    
    return false;
}
