/**
 * =====================================================================================
 * @file        bsp_log_manager.c
 * @brief       基于串口的日志管理系统实现
 * @author      23Elapse
 * @version     1.0
 * @date        2025-09-02
 * @note        提供分级日志、时间戳、模块标识等功能
 * =====================================================================================
 */

#include "bsp_log_manager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "n32g45x.h"
#include <string.h>
#include <stdio.h>

// 内部结构定义
typedef struct {
    bool initialized;                   // 初始化状态
    LogConfig_t config;                 // 日志配置
    LogStatistics_t stats;              // 统计信息
    char log_buffer[LOG_MAX_LINE_LENGTH]; // 格式化缓冲区
    char timestamp_buffer[LOG_TIMESTAMP_BUFFER_SIZE]; // 时间戳缓冲区
    SemaphoreHandle_t mutex;            // 互斥锁
    char module_names[LOG_MODULE_COUNT][LOG_MODULE_NAME_MAX_LEN + 1]; // 自定义模块名
} LogManager_t;

// 全局日志管理器实例
static LogManager_t g_log_manager = {0};

// 默认配置
static const LogConfig_t default_config = {
    .global_level = LOG_LEVEL_INFO,
    .module_levels = {
        [LOG_MODULE_SYSTEM] = LOG_LEVEL_INFO,
        [LOG_MODULE_UART] = LOG_LEVEL_INFO,
        [LOG_MODULE_NETWORK] = LOG_LEVEL_INFO,
        [LOG_MODULE_SENSOR] = LOG_LEVEL_INFO,
        [LOG_MODULE_STORAGE] = LOG_LEVEL_INFO,
        [LOG_MODULE_PROTOCOL] = LOG_LEVEL_INFO,
        [LOG_MODULE_APP] = LOG_LEVEL_INFO,
        [LOG_MODULE_TEST] = LOG_LEVEL_DEBUG,
    },
    .timestamp_type = LOG_TIMESTAMP_TICK,
    .output_targets = LOG_OUTPUT_UART,
    .enable_colors = false,
    .enable_file_line = false,
    .buffer_size = LOG_BUFFER_SIZE,
    .max_line_length = LOG_MAX_LINE_LENGTH
};

// 日志级别颜色
static const char* level_colors[] = {
    LOG_COLOR_CYAN,     // LOG_LEVEL_DEBUG = 0
    LOG_COLOR_GREEN,    // LOG_LEVEL_INFO = 1  
    LOG_COLOR_YELLOW,   // LOG_LEVEL_WARN = 2
    LOG_COLOR_RED,      // LOG_LEVEL_ERROR = 3
    LOG_COLOR_MAGENTA   // LOG_LEVEL_FATAL = 4
};

/**
 * @brief 格式化时间戳
 * @param buffer 输出缓冲区
 * @param size 缓冲区大小
 * @param type 时间戳类型
 * @return 时间戳字符串长度
 */
static int format_timestamp(char* buffer, size_t size, LogTimestampType_t type) {
    static uint32_t boot_sequence = 0;  // 启动序列计数器
    uint32_t timestamp_value;
    
    // 如果调度器已启动，使用系统tick；否则使用启动序列
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        timestamp_value = xTaskGetTickCount();
    } else {
        timestamp_value = boot_sequence++;
    }
    
    switch (type) {
        case LOG_TIMESTAMP_TICK:
            return snprintf(buffer, size, "[%010lu]", timestamp_value);
            
        case LOG_TIMESTAMP_MS:
            if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
                return snprintf(buffer, size, "[%lu ms]", timestamp_value * portTICK_PERIOD_MS);
            } else {
                return snprintf(buffer, size, "[BOOT-%lu]", timestamp_value);
            }
            
        case LOG_TIMESTAMP_RTC:
            // TODO: 实现RTC时间格式化
            return snprintf(buffer, size, "[RTC-TODO]");
            
        case LOG_TIMESTAMP_NONE:
        default:
            return 0;
    }
}

/**
 * @brief 格式化文件名和行号
 * @param buffer 输出缓冲区
 * @param size 缓冲区大小
 * @param file 文件名
 * @param line 行号
 * @return 格式化字符串长度
 */
static int format_file_line(char* buffer, size_t size, const char* file, int line) {
    if (!file || line <= 0) {
        return 0;
    }
    
    // 提取文件名（去掉路径）
    const char* filename = strrchr(file, '/');
    if (!filename) {
        filename = strrchr(file, '\\');
    }
    if (filename) {
        filename++; // 跳过分隔符
    } else {
        filename = file;
    }
    
    return snprintf(buffer, size, " [%s:%d]", filename, line);
}

/**
 * @brief 输出日志到串口
 * @param message 格式化后的日志消息
 */
static void output_to_uart(const char* message) {
    UART_RingBuffer_SendString(UART_ID_LOG, message, LOG_OUTPUT_TIMEOUT_MS);
}

/**
 * @brief 锁定日志管理器
 * @return true成功，false失败
 */
static bool lock_log_manager(void) {
    if (!g_log_manager.mutex) {
        return false;
    }
    return (xSemaphoreTake(g_log_manager.mutex, pdMS_TO_TICKS(100)) == pdTRUE);
}

/**
 * @brief 解锁日志管理器
 */
static void unlock_log_manager(void) {
    if (g_log_manager.mutex) {
        xSemaphoreGive(g_log_manager.mutex);
    }
}

/**
 * @brief 初始化日志管理系统
 * @param config 日志配置，传NULL使用默认配置
 * @return 初始化结果
 */
bool LogManager_Init(const LogConfig_t* config) {
    if (g_log_manager.initialized) {
        return true; // 已经初始化
    }
    
    // 使用默认配置或用户配置
    if (config) {
        memcpy(&g_log_manager.config, config, sizeof(LogConfig_t));
    } else {
        memcpy(&g_log_manager.config, &default_config, sizeof(LogConfig_t));
    }
    
    // 初始化模块名 - 使用直接赋值方式避免指针问题
    strcpy(g_log_manager.module_names[0], "SYS");
    strcpy(g_log_manager.module_names[1], "UART");
    strcpy(g_log_manager.module_names[2], "NET");
    strcpy(g_log_manager.module_names[3], "SENSOR");
    strcpy(g_log_manager.module_names[4], "STORAGE");
    strcpy(g_log_manager.module_names[5], "PROTO");
    strcpy(g_log_manager.module_names[6], "APP");
    strcpy(g_log_manager.module_names[7], "TEST");
    
    // 创建互斥锁
    g_log_manager.mutex = xSemaphoreCreateMutex();
    if (!g_log_manager.mutex) {
        return false;
    }
    
    // 初始化LOG串口
    if (UART_RingBuffer_Init(UART_ID_LOG, 115200) != UART_OK) {
        vSemaphoreDelete(g_log_manager.mutex);
        g_log_manager.mutex = NULL;
        return false;
    }
    
    // 清零统计信息
    memset(&g_log_manager.stats, 0, sizeof(LogStatistics_t));
    
    g_log_manager.initialized = true;
    
    // 输出初始化成功信息
    SYS_LOG_INFO("Log Manager initialized successfully");
    LogManager_PrintSystemInfo();
    
    return true;
}

/**
 * @brief 反初始化日志管理系统
 */
void LogManager_Deinit(void) {
    if (!g_log_manager.initialized) {
        return;
    }
    
    SYS_LOG_INFO("Log Manager deinitializing...");
    
    // 刷新缓冲区
    LogManager_Flush();
    
    // 反初始化串口
    UART_RingBuffer_Deinit(UART_ID_LOG);
    
    // 删除互斥锁
    if (g_log_manager.mutex) {
        vSemaphoreDelete(g_log_manager.mutex);
        g_log_manager.mutex = NULL;
    }
    
    g_log_manager.initialized = false;
}

/**
 * @brief 检查日志级别是否启用
 * @param level 日志级别
 * @param module 模块ID
 * @return true如果启用，false如果禁用
 */
bool LogManager_IsLevelEnabled(LogLevel_t level, LogModule_t module) {
    if (!g_log_manager.initialized || level >= LOG_LEVEL_OFF) {
        return false;
    }
    
    // 检查全局级别
    if (level < g_log_manager.config.global_level) {
        return false;
    }
    
    // 检查模块级别
    if (module < LOG_MODULE_COUNT && level < g_log_manager.config.module_levels[module]) {
        return false;
    }
    
    return true;
}

/**
 * @brief 输出日志（带可变参数列表）
 * @param level 日志级别
 * @param module 模块ID
 * @param file 文件名
 * @param line 行号
 * @param format 格式字符串
 * @param args 参数列表
 */
void LogManager_OutputV(LogLevel_t level, LogModule_t module, const char* file, int line, const char* format, va_list args) {
    if (!g_log_manager.initialized || !LogManager_IsLevelEnabled(level, module)) {
        return;
    }
    
    if (!lock_log_manager()) {
        g_log_manager.stats.dropped_logs++;
        return;
    }
    
    // 更新统计信息
    g_log_manager.stats.total_logs++;
    if (level < LOG_LEVEL_OFF) {
        g_log_manager.stats.logs_by_level[level]++;
    }
    if (module < LOG_MODULE_COUNT) {
        g_log_manager.stats.logs_by_module[module]++;
    }
    
    // 构建日志消息
    char* buffer = g_log_manager.log_buffer;
    int pos = 0;
    int remaining = LOG_MAX_LINE_LENGTH - 1; // 保留一个字符给'\0'
    
    // 添加颜色代码
    if (g_log_manager.config.enable_colors && level < LOG_LEVEL_OFF) {
        int len = snprintf(buffer + pos, remaining, "%s", level_colors[level]);
        pos += len;
        remaining -= len;
    }
    
    // 添加时间戳
    if (g_log_manager.config.timestamp_type != LOG_TIMESTAMP_NONE && remaining > 0) {
        int len = format_timestamp(g_log_manager.timestamp_buffer, 
                                 LOG_TIMESTAMP_BUFFER_SIZE, 
                                 g_log_manager.config.timestamp_type);
        if (len > 0 && len < remaining) {
            memcpy(buffer + pos, g_log_manager.timestamp_buffer, len);
            pos += len;
            remaining -= len;
        }
    }
    
    // 添加级别和模块信息
    if (remaining > 0) {
        const char* level_str = LogManager_GetLevelString(level);
        const char* module_str = (module < LOG_MODULE_COUNT) ? g_log_manager.module_names[module] : "UNKNOWN";
        
        int len = snprintf(buffer + pos, remaining, " [%s][%s] ", level_str, module_str);
        if (len > 0 && len < remaining) {
            pos += len;
            remaining -= len;
        }
    }
    
    // 添加用户消息
    if (remaining > 0) {
        int len = vsnprintf(buffer + pos, remaining, format, args);
        if (len > 0) {
            pos += (len < remaining) ? len : remaining - 1;
            remaining = LOG_MAX_LINE_LENGTH - 1 - pos;
        }
    }
    
    // 添加文件名和行号
    if (g_log_manager.config.enable_file_line && file && line > 0 && remaining > 0) {
        int len = format_file_line(buffer + pos, remaining, file, line);
        if (len > 0 && len < remaining) {
            pos += len;
            remaining -= len;
        }
    }
    
    // 添加颜色重置代码
    if (g_log_manager.config.enable_colors && remaining > 0) {
        int len = snprintf(buffer + pos, remaining, "%s", LOG_COLOR_RESET);
        if (len > 0 && len < remaining) {
            pos += len;
            remaining -= len;
        }
    }
    
    // 添加换行符
    if (remaining > 0) {
        int len = snprintf(buffer + pos, remaining, "\r\n");
        if (len > 0 && len < remaining) {
            pos += len;
        }
    }
    
    // 确保字符串结束
    buffer[LOG_MAX_LINE_LENGTH - 1] = '\0';
    
    // 输出到目标
    if (g_log_manager.config.output_targets & LOG_OUTPUT_UART) {
        output_to_uart(buffer);
    }
    
    unlock_log_manager();
}

/**
 * @brief 输出日志（核心函数）
 * @param level 日志级别
 * @param module 模块ID
 * @param file 文件名（可为NULL）
 * @param line 行号
 * @param format 格式字符串
 * @param ... 可变参数
 */
void LogManager_Output(LogLevel_t level, LogModule_t module, const char* file, int line, const char* format, ...) {
    va_list args;
    va_start(args, format);
    LogManager_OutputV(level, module, file, line, format, args);
    va_end(args);
}

/**
 * @brief 设置全局日志级别
 * @param level 日志级别
 */
void LogManager_SetGlobalLevel(LogLevel_t level) {
    if (!g_log_manager.initialized || level > LOG_LEVEL_OFF) {
        return;
    }
    
    if (lock_log_manager()) {
        g_log_manager.config.global_level = level;
        unlock_log_manager();
        
        SYS_LOG_INFO("Global log level set to %s", LogManager_GetLevelString(level));
    }
}

/**
 * @brief 设置模块日志级别
 * @param module 模块ID
 * @param level 日志级别
 */
void LogManager_SetModuleLevel(LogModule_t module, LogLevel_t level) {
    if (!g_log_manager.initialized || module >= LOG_MODULE_COUNT || level > LOG_LEVEL_OFF) {
        return;
    }
    
    if (lock_log_manager()) {
        g_log_manager.config.module_levels[module] = level;
        unlock_log_manager();
        
        SYS_LOG_INFO("Module %s log level set to %s", 
                    LogManager_GetModuleString(module), 
                    LogManager_GetLevelString(level));
    }
}

/**
 * @brief 获取全局日志级别
 * @return 当前全局日志级别
 */
LogLevel_t LogManager_GetGlobalLevel(void) {
    return g_log_manager.initialized ? g_log_manager.config.global_level : LOG_LEVEL_OFF;
}

/**
 * @brief 获取模块日志级别
 * @param module 模块ID
 * @return 模块日志级别
 */
LogLevel_t LogManager_GetModuleLevel(LogModule_t module) {
    if (!g_log_manager.initialized || module >= LOG_MODULE_COUNT) {
        return LOG_LEVEL_OFF;
    }
    return g_log_manager.config.module_levels[module];
}

/**
 * @brief 启用/禁用颜色输出
 * @param enable true启用，false禁用
 */
void LogManager_EnableColors(bool enable) {
    if (!g_log_manager.initialized) {
        return;
    }
    
    if (lock_log_manager()) {
        g_log_manager.config.enable_colors = enable;
        unlock_log_manager();
        
        SYS_LOG_INFO("Color output %s", enable ? "enabled" : "disabled");
    }
}

/**
 * @brief 启用/禁用文件名行号显示
 * @param enable true启用，false禁用
 */
void LogManager_EnableFileLine(bool enable) {
    if (!g_log_manager.initialized) {
        return;
    }
    
    if (lock_log_manager()) {
        g_log_manager.config.enable_file_line = enable;
        unlock_log_manager();
        
        SYS_LOG_INFO("File/line display %s", enable ? "enabled" : "disabled");
    }
}

/**
 * @brief 设置时间戳类型
 * @param type 时间戳类型
 */
void LogManager_SetTimestampType(LogTimestampType_t type) {
    if (!g_log_manager.initialized || type > LOG_TIMESTAMP_RTC) {
        return;
    }
    
    if (lock_log_manager()) {
        g_log_manager.config.timestamp_type = type;
        unlock_log_manager();
        
        const char* type_names[] = {"NONE", "TICK", "MS", "RTC"};
        SYS_LOG_INFO("Timestamp type set to %s", type_names[type]);
    }
}

/**
 * @brief 获取日志统计信息
 * @param stats 统计信息结构体指针
 */
void LogManager_GetStatistics(LogStatistics_t* stats) {
    if (!g_log_manager.initialized || !stats) {
        return;
    }
    
    if (lock_log_manager()) {
        memcpy(stats, &g_log_manager.stats, sizeof(LogStatistics_t));
        unlock_log_manager();
    }
}

/**
 * @brief 清除日志统计信息
 */
void LogManager_ClearStatistics(void) {
    if (!g_log_manager.initialized) {
        return;
    }
    
    if (lock_log_manager()) {
        memset(&g_log_manager.stats, 0, sizeof(LogStatistics_t));
        unlock_log_manager();
        
        SYS_LOG_INFO("Log statistics cleared");
    }
}

/**
 * @brief 刷新日志缓冲区
 */
void LogManager_Flush(void) {
    if (!g_log_manager.initialized) {
        return;
    }
    
    // 等待串口发送完成
    vTaskDelay(pdMS_TO_TICKS(50));
}

/**
 * @brief 输出十六进制数据
 * @param level 日志级别
 * @param module 模块ID
 * @param data 数据指针
 * @param length 数据长度
 * @param format 描述格式字符串
 * @param ... 可变参数
 */
void LogManager_HexDump(LogLevel_t level, LogModule_t module, const uint8_t* data, uint32_t length, const char* format, ...) {
    if (!g_log_manager.initialized || !LogManager_IsLevelEnabled(level, module) || !data || length == 0) {
        return;
    }
    
    // 输出描述信息
    if (format) {
        va_list args;
        va_start(args, format);
        LogManager_OutputV(level, module, NULL, 0, format, args);
        va_end(args);
    }
    
    // 输出十六进制数据
    char hex_buffer[64];
    const uint32_t bytes_per_line = 16;
    
    for (uint32_t i = 0; i < length; i += bytes_per_line) {
        int pos = 0;
        
        // 输出地址
        pos += snprintf(hex_buffer + pos, sizeof(hex_buffer) - pos, "%04X: ", i);
        
        // 输出十六进制数据
        uint32_t line_length = (length - i > bytes_per_line) ? bytes_per_line : (length - i);
        for (uint32_t j = 0; j < line_length; j++) {
            pos += snprintf(hex_buffer + pos, sizeof(hex_buffer) - pos, "%02X ", data[i + j]);
        }
        
        // 补充空格对齐
        for (uint32_t j = line_length; j < bytes_per_line; j++) {
            pos += snprintf(hex_buffer + pos, sizeof(hex_buffer) - pos, "   ");
        }
        
        // 输出ASCII字符
        pos += snprintf(hex_buffer + pos, sizeof(hex_buffer) - pos, " |");
        for (uint32_t j = 0; j < line_length; j++) {
            char c = data[i + j];
            pos += snprintf(hex_buffer + pos, sizeof(hex_buffer) - pos, "%c", 
                          (c >= 32 && c <= 126) ? c : '.');
        }
        pos += snprintf(hex_buffer + pos, sizeof(hex_buffer) - pos, "|");
        
        LogManager_Output(level, module, NULL, 0, "%s", hex_buffer);
    }
}

/**
 * @brief 获取日志级别字符串
 * @param level 日志级别
 * @return 级别字符串
 */
const char* LogManager_GetLevelString(LogLevel_t level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

/**
 * @brief 获取模块名字符串
 * @param module 模块ID
 * @return 模块名字符串
 */
const char* LogManager_GetModuleString(LogModule_t module) {
    if (module < LOG_MODULE_COUNT) {
        return g_log_manager.module_names[module];
    }
    return "UNKNOWN";
}

/**
 * @brief 设置自定义模块名
 * @param module 模块ID
 * @param name 模块名字符串
 */
void LogManager_SetModuleName(LogModule_t module, const char* name) {
    if (!g_log_manager.initialized || module >= LOG_MODULE_COUNT || !name) {
        return;
    }
    
    if (lock_log_manager()) {
        strncpy(g_log_manager.module_names[module], name, LOG_MODULE_NAME_MAX_LEN);
        g_log_manager.module_names[module][LOG_MODULE_NAME_MAX_LEN] = '\0';
        unlock_log_manager();
        
        SYS_LOG_INFO("Module %d name set to '%s'", module, name);
    }
}

/**
 * @brief 输出系统信息
 */
void LogManager_PrintSystemInfo(void) {
    if (!g_log_manager.initialized) {
        return;
    }
    
    SYS_LOG_INFO("=== System Information ===");
    SYS_LOG_INFO("FreeRTOS Version: %s", tskKERNEL_VERSION_NUMBER);
    SYS_LOG_INFO("CPU Frequency: %lu Hz", SystemCoreClock);
    SYS_LOG_INFO("Tick Rate: %d Hz", configTICK_RATE_HZ);
    SYS_LOG_INFO("Heap Size: %d bytes", configTOTAL_HEAP_SIZE);
    SYS_LOG_INFO("Free Heap: %d bytes", xPortGetFreeHeapSize());
    SYS_LOG_INFO("Min Free Heap: %d bytes", xPortGetMinimumEverFreeHeapSize());
    SYS_LOG_INFO("Log Manager: Initialized");
    SYS_LOG_INFO("==========================");
}

/**
 * @brief 输出日志统计信息
 */
void LogManager_PrintStatistics(void) {
    if (!g_log_manager.initialized) {
        return;
    }
    
    LogStatistics_t stats;
    LogManager_GetStatistics(&stats);
    
    SYS_LOG_INFO("=== Log Statistics ===");
    SYS_LOG_INFO("Total Logs: %lu", stats.total_logs);
    SYS_LOG_INFO("Dropped Logs: %lu", stats.dropped_logs);
    SYS_LOG_INFO("Buffer Overflows: %lu", stats.buffer_overflows);
    
    SYS_LOG_INFO("Logs by Level:");
    for (int i = 0; i < LOG_LEVEL_OFF; i++) {
        SYS_LOG_INFO("  %s: %lu", LogManager_GetLevelString((LogLevel_t)i), stats.logs_by_level[i]);
    }
    
    SYS_LOG_INFO("Logs by Module:");
    for (int i = 0; i < LOG_MODULE_COUNT; i++) {
        SYS_LOG_INFO("  %s: %lu", LogManager_GetModuleString((LogModule_t)i), stats.logs_by_module[i]);
    }
    SYS_LOG_INFO("=====================");
}
