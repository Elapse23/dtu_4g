/**
 * =====================================================================================
 * @file        bsp_log_manager.h
 * @brief       基于串口的日志管理系统头文件
 * @author      23Elapse
 * @version     1.0
 * @date        2025-09-02
 * @note        提供分级日志、时间戳、模块标识等功能
 * =====================================================================================
 */

#ifndef __BSP_LOG_MANAGER_H
#define __BSP_LOG_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "bsp_uart_ring_buffer.h"

// 日志级别定义
typedef enum {
    LOG_LEVEL_DEBUG = 0,    // 调试信息
    LOG_LEVEL_INFO,         // 一般信息
    LOG_LEVEL_WARN,         // 警告信息
    LOG_LEVEL_ERROR,        // 错误信息
    LOG_LEVEL_FATAL,        // 致命错误
    LOG_LEVEL_OFF           // 关闭日志
} LogLevel_t;

// 日志模块定义
typedef enum {
    LOG_MODULE_SYSTEM = 0,  // 系统模块
    LOG_MODULE_UART,        // 串口模块
    LOG_MODULE_NETWORK,     // 网络模块
    LOG_MODULE_SENSOR,      // 传感器模块
    LOG_MODULE_STORAGE,     // 存储模块
    LOG_MODULE_PROTOCOL,    // 协议模块
    LOG_MODULE_APP,         // 应用模块
    LOG_MODULE_TEST,        // 测试模块
    LOG_MODULE_COUNT        // 模块总数
} LogModule_t;

// 日志时间戳类型
typedef enum {
    LOG_TIMESTAMP_NONE = 0, // 无时间戳
    LOG_TIMESTAMP_TICK,     // 系统tick
    LOG_TIMESTAMP_MS,       // 毫秒时间戳
    LOG_TIMESTAMP_RTC       // RTC时间
} LogTimestampType_t;

// 日志输出目标
typedef enum {
    LOG_OUTPUT_UART = 0x01,     // 串口输出
    LOG_OUTPUT_FILE = 0x02,     // 文件输出（预留）
    LOG_OUTPUT_NETWORK = 0x04,  // 网络输出（预留）
    LOG_OUTPUT_ALL = 0xFF       // 所有输出
} LogOutputTarget_t;

// 日志配置结构体
typedef struct {
    LogLevel_t global_level;            // 全局日志级别
    LogLevel_t module_levels[LOG_MODULE_COUNT]; // 各模块日志级别
    LogTimestampType_t timestamp_type;  // 时间戳类型
    uint8_t output_targets;             // 输出目标（位掩码）
    bool enable_colors;                 // 是否启用颜色输出
    bool enable_file_line;              // 是否显示文件名和行号
    uint32_t buffer_size;               // 日志缓冲区大小
    uint32_t max_line_length;           // 单行最大长度
} LogConfig_t;

// 日志统计信息
typedef struct {
    uint32_t total_logs;                // 总日志数
    uint32_t logs_by_level[LOG_LEVEL_OFF]; // 各级别日志数
    uint32_t logs_by_module[LOG_MODULE_COUNT]; // 各模块日志数
    uint32_t dropped_logs;              // 丢弃的日志数
    uint32_t buffer_overflows;          // 缓冲区溢出次数
} LogStatistics_t;

// 配置宏定义
#define LOG_MAX_LINE_LENGTH         256     // 单行日志最大长度
#define LOG_BUFFER_SIZE             1024    // 日志缓冲区大小
#define LOG_MODULE_NAME_MAX_LEN     8       // 模块名最大长度
#define LOG_TIMESTAMP_BUFFER_SIZE   32      // 时间戳缓冲区大小

// 默认超时时间
#define LOG_OUTPUT_TIMEOUT_MS       100     // 日志输出超时时间

// 颜色代码定义（ANSI转义序列）
#define LOG_COLOR_RESET     "\033[0m"
#define LOG_COLOR_RED       "\033[31m"
#define LOG_COLOR_GREEN     "\033[32m"
#define LOG_COLOR_YELLOW    "\033[33m"
#define LOG_COLOR_BLUE      "\033[34m"
#define LOG_COLOR_MAGENTA   "\033[35m"
#define LOG_COLOR_CYAN      "\033[36m"
#define LOG_COLOR_WHITE     "\033[37m"

// === 核心API函数 ===

/**
 * @brief 初始化日志管理系统
 * @param config 日志配置，传NULL使用默认配置
 * @return 初始化结果
 */
bool LogManager_Init(const LogConfig_t* config);

/**
 * @brief 反初始化日志管理系统
 */
void LogManager_Deinit(void);

/**
 * @brief 输出日志（核心函数）
 * @param level 日志级别
 * @param module 模块ID
 * @param file 文件名（可为NULL）
 * @param line 行号
 * @param format 格式字符串
 * @param ... 可变参数
 */
void LogManager_Output(LogLevel_t level, LogModule_t module, const char* file, int line, const char* format, ...);

/**
 * @brief 输出日志（带可变参数列表）
 * @param level 日志级别
 * @param module 模块ID
 * @param file 文件名
 * @param line 行号
 * @param format 格式字符串
 * @param args 参数列表
 */
void LogManager_OutputV(LogLevel_t level, LogModule_t module, const char* file, int line, const char* format, va_list args);

/**
 * @brief 设置全局日志级别
 * @param level 日志级别
 */
void LogManager_SetGlobalLevel(LogLevel_t level);

/**
 * @brief 设置模块日志级别
 * @param module 模块ID
 * @param level 日志级别
 */
void LogManager_SetModuleLevel(LogModule_t module, LogLevel_t level);

/**
 * @brief 获取全局日志级别
 * @return 当前全局日志级别
 */
LogLevel_t LogManager_GetGlobalLevel(void);

/**
 * @brief 获取模块日志级别
 * @param module 模块ID
 * @return 模块日志级别
 */
LogLevel_t LogManager_GetModuleLevel(LogModule_t module);

/**
 * @brief 启用/禁用颜色输出
 * @param enable true启用，false禁用
 */
void LogManager_EnableColors(bool enable);

/**
 * @brief 启用/禁用文件名行号显示
 * @param enable true启用，false禁用
 */
void LogManager_EnableFileLine(bool enable);

/**
 * @brief 设置时间戳类型
 * @param type 时间戳类型
 */
void LogManager_SetTimestampType(LogTimestampType_t type);

/**
 * @brief 获取日志统计信息
 * @param stats 统计信息结构体指针
 */
void LogManager_GetStatistics(LogStatistics_t* stats);

/**
 * @brief 清除日志统计信息
 */
void LogManager_ClearStatistics(void);

/**
 * @brief 刷新日志缓冲区
 */
void LogManager_Flush(void);

/**
 * @brief 检查日志级别是否启用
 * @param level 日志级别
 * @param module 模块ID
 * @return true如果启用，false如果禁用
 */
bool LogManager_IsLevelEnabled(LogLevel_t level, LogModule_t module);

// === 便捷宏定义 ===

// 基本日志宏（不带文件名行号）
#define LOG_DEBUG(module, fmt, ...)   LogManager_Output(LOG_LEVEL_DEBUG, module, NULL, 0, fmt, ##__VA_ARGS__)
#define LOG_INFO(module, fmt, ...)    LogManager_Output(LOG_LEVEL_INFO, module, NULL, 0, fmt, ##__VA_ARGS__)
#define LOG_WARN(module, fmt, ...)    LogManager_Output(LOG_LEVEL_WARN, module, NULL, 0, fmt, ##__VA_ARGS__)
#define LOG_ERROR(module, fmt, ...)   LogManager_Output(LOG_LEVEL_ERROR, module, NULL, 0, fmt, ##__VA_ARGS__)
#define LOG_FATAL(module, fmt, ...)   LogManager_Output(LOG_LEVEL_FATAL, module, NULL, 0, fmt, ##__VA_ARGS__)

// 详细日志宏（带文件名行号）
#define LOG_DEBUG_EX(module, fmt, ...)   LogManager_Output(LOG_LEVEL_DEBUG, module, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO_EX(module, fmt, ...)    LogManager_Output(LOG_LEVEL_INFO, module, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN_EX(module, fmt, ...)    LogManager_Output(LOG_LEVEL_WARN, module, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR_EX(module, fmt, ...)   LogManager_Output(LOG_LEVEL_ERROR, module, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_FATAL_EX(module, fmt, ...)   LogManager_Output(LOG_LEVEL_FATAL, module, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

// 系统模块专用宏
#define SYS_LOG_DEBUG(fmt, ...)     LOG_DEBUG(LOG_MODULE_SYSTEM, fmt, ##__VA_ARGS__)
#define SYS_LOG_INFO(fmt, ...)      LOG_INFO(LOG_MODULE_SYSTEM, fmt, ##__VA_ARGS__)
#define SYS_LOG_WARN(fmt, ...)      LOG_WARN(LOG_MODULE_SYSTEM, fmt, ##__VA_ARGS__)
#define SYS_LOG_ERROR(fmt, ...)     LOG_ERROR(LOG_MODULE_SYSTEM, fmt, ##__VA_ARGS__)
#define SYS_LOG_FATAL(fmt, ...)     LOG_FATAL(LOG_MODULE_SYSTEM, fmt, ##__VA_ARGS__)

// 串口模块专用宏
#define UART_LOG_DEBUG(fmt, ...)    LOG_DEBUG(LOG_MODULE_UART, fmt, ##__VA_ARGS__)
#define UART_LOG_INFO(fmt, ...)     LOG_INFO(LOG_MODULE_UART, fmt, ##__VA_ARGS__)
#define UART_LOG_WARN(fmt, ...)     LOG_WARN(LOG_MODULE_UART, fmt, ##__VA_ARGS__)
#define UART_LOG_ERROR(fmt, ...)    LOG_ERROR(LOG_MODULE_UART, fmt, ##__VA_ARGS__)

// 网络模块专用宏
#define NET_LOG_DEBUG(fmt, ...)     LOG_DEBUG(LOG_MODULE_NETWORK, fmt, ##__VA_ARGS__)
#define NET_LOG_INFO(fmt, ...)      LOG_INFO(LOG_MODULE_NETWORK, fmt, ##__VA_ARGS__)
#define NET_LOG_WARN(fmt, ...)      LOG_WARN(LOG_MODULE_NETWORK, fmt, ##__VA_ARGS__)
#define NET_LOG_ERROR(fmt, ...)     LOG_ERROR(LOG_MODULE_NETWORK, fmt, ##__VA_ARGS__)

// 应用模块专用宏
#define APP_LOG_DEBUG(fmt, ...)     LOG_DEBUG(LOG_MODULE_APP, fmt, ##__VA_ARGS__)
#define APP_LOG_INFO(fmt, ...)      LOG_INFO(LOG_MODULE_APP, fmt, ##__VA_ARGS__)
#define APP_LOG_WARN(fmt, ...)      LOG_WARN(LOG_MODULE_APP, fmt, ##__VA_ARGS__)
#define APP_LOG_ERROR(fmt, ...)     LOG_ERROR(LOG_MODULE_APP, fmt, ##__VA_ARGS__)

// 条件日志宏
#define LOG_DEBUG_IF(cond, module, fmt, ...)    do { if (cond) LOG_DEBUG(module, fmt, ##__VA_ARGS__); } while(0)
#define LOG_INFO_IF(cond, module, fmt, ...)     do { if (cond) LOG_INFO(module, fmt, ##__VA_ARGS__); } while(0)
#define LOG_WARN_IF(cond, module, fmt, ...)     do { if (cond) LOG_WARN(module, fmt, ##__VA_ARGS__); } while(0)
#define LOG_ERROR_IF(cond, module, fmt, ...)    do { if (cond) LOG_ERROR(module, fmt, ##__VA_ARGS__); } while(0)

// 断言宏
#define LOG_ASSERT(cond, module, fmt, ...)      do { if (!(cond)) { LOG_FATAL(module, "ASSERT FAILED: " fmt, ##__VA_ARGS__); while(1); } } while(0)

// 函数入口/出口日志宏
#define LOG_FUNC_ENTER(module)      LOG_DEBUG(module, "ENTER %s", __FUNCTION__)
#define LOG_FUNC_EXIT(module)       LOG_DEBUG(module, "EXIT %s", __FUNCTION__)

// 十六进制数据日志宏
#define LOG_HEX_DUMP(module, data, len, fmt, ...)   LogManager_HexDump(LOG_LEVEL_INFO, module, data, len, fmt, ##__VA_ARGS__)

// === 扩展功能函数 ===

/**
 * @brief 输出十六进制数据
 * @param level 日志级别
 * @param module 模块ID
 * @param data 数据指针
 * @param length 数据长度
 * @param format 描述格式字符串
 * @param ... 可变参数
 */
void LogManager_HexDump(LogLevel_t level, LogModule_t module, const uint8_t* data, uint32_t length, const char* format, ...);

/**
 * @brief 获取日志级别字符串
 * @param level 日志级别
 * @return 级别字符串
 */
const char* LogManager_GetLevelString(LogLevel_t level);

/**
 * @brief 获取模块名字符串
 * @param module 模块ID
 * @return 模块名字符串
 */
const char* LogManager_GetModuleString(LogModule_t module);

/**
 * @brief 设置自定义模块名
 * @param module 模块ID
 * @param name 模块名字符串
 */
void LogManager_SetModuleName(LogModule_t module, const char* name);

/**
 * @brief 输出系统信息
 */
void LogManager_PrintSystemInfo(void);

/**
 * @brief 输出日志统计信息
 */
void LogManager_PrintStatistics(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_LOG_MANAGER_H */
