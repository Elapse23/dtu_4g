/**
 * =====================================================================================
 * @file        tsk_watchdog.h
 * @brief       系统任务看门狗头文件
 * @author      23Elapse
 * @version     1.0
 * @date        2025-09-08
 * @note        监控关键任务状态，防止系统死锁
 * =====================================================================================
 */

#ifndef __TSK_WATCHDOG_H
#define __TSK_WATCHDOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* ========================== 公共函数声明 ========================== */

/**
 * @brief 初始化任务看门狗
 * @return bool 初始化是否成功
 */
bool TaskWatchdog_Init(void);

/**
 * @brief 启动任务看门狗
 * @return bool 启动是否成功
 */
bool TaskWatchdog_Start(void);

/**
 * @brief 停止任务看门狗
 */
void TaskWatchdog_Stop(void);

/**
 * @brief 检查系统是否死锁
 * @return bool true-系统正常，false-可能死锁
 */
bool TaskWatchdog_CheckSystem(void);

#ifdef __cplusplus
}
#endif

#endif /* __TSK_WATCHDOG_H */
