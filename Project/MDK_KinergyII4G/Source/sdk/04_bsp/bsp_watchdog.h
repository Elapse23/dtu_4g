/**
 * =====================================================================================
 * @file        bsp_watchdog.h
 * @brief       独立看门狗管理头文件
 * @author      23Elapse & GitHub Copilot
 * @version     1.0
 * @date        2025-09-04
 * @note        提供统一的看门狗操作接口
 * =====================================================================================
 */

#ifndef BSP_WATCHDOG_H
#define BSP_WATCHDOG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================= 头文件包含 ============================= */
#include "n32g45x.h"

/* ============================= 宏定义 ============================= */

/**
 * @brief 看门狗重载键值定义
 */
#define KEY_ReloadKey    0xAAAA

/**
 * @brief 独立看门狗喂狗宏 - 暂时禁用用于调试重启问题
 * @note 用于在任务中定期重载看门狗计数器，防止系统重启
 */
#define MACRO_IWDG_RELOAD()      do { /* 暂时禁用看门狗 */ } while(0)

/**
 * @brief 推荐的看门狗喂狗间隔（毫秒）
 * @note 独立看门狗超时时间约为31秒，建议在2-10秒内喂狗
 */
#define WATCHDOG_FEED_INTERVAL_MS    2000

#ifdef __cplusplus
}
#endif

#endif /* BSP_WATCHDOG_H */
