#pragma once

#include <cstdio>

#define YAZPGP_LOGGER_LEVEL_DEBUG 0
#define YAZPGP_LOGGER_LEVEL_INFO 1
#define YAZPGP_LOGGER_LEVEL_WARNING 2
#define YAZPGP_LOGGER_LEVEL_ERROR 3
#define YAZPGP_LOGGER_LEVEL_FATAL 4

#define YAZPGP_LOGGER_COLOR_RESET "\033[0m"
#define YAZPGP_LOGGER_COLOR_RED "\033[31m"
#define YAZPGP_LOGGER_COLOR_GREEN "\033[32m"
#define YAZPGP_LOGGER_COLOR_YELLOW "\033[33m"
#define YAZPGP_LOGGER_COLOR_BLUE "\033[34m"

#define YAZPGP_LOGGER_OUTPUT_STDOUT stdout
#define YAZPGP_LOGGER_OUTPUT_STDERR stderr

[[maybe_unused]]
static const char* YAZPGP_LEVEL_STRINGS[] = {
    "[DEBUG]",
    "[INFO]",
    "[WARNING]",
    "[ERROR]",
    "[FATAL]"
};

[[maybe_unused]]
static const char* YAZPGP_LEVEL_COLORS[] = { 
    YAZPGP_LOGGER_COLOR_BLUE,
    YAZPGP_LOGGER_COLOR_GREEN,
    YAZPGP_LOGGER_COLOR_YELLOW,
    YAZPGP_LOGGER_COLOR_RED,
    YAZPGP_LOGGER_COLOR_RED
};

#ifndef YAZPGP_LOGGER_LEVEL_SETTINGS
    #define YAZPGP_LOGGER_LEVEL_SETTINGS YAZPGP_LOGGER_LEVEL_DEBUG
#endif

#define YAZPGP_LOGGER_LOG(file,level,fmt,...) do { \
    if (level >= YAZPGP_LOGGER_LEVEL_SETTINGS) { \
        std::fprintf(file, "%s%s%s ", YAZPGP_LEVEL_COLORS[level] , YAZPGP_LEVEL_STRINGS[level], YAZPGP_LOGGER_COLOR_RESET); \
        std::fprintf(file, fmt, ##__VA_ARGS__); \
        std::fprintf(file, "\n"); \
    } \
} while (0)

#define YAZPGP_LOG_DEBUG(fmt,...) YAZPGP_LOGGER_LOG(YAZPGP_LOGGER_OUTPUT_STDOUT, YAZPGP_LOGGER_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define YAZPGP_LOG_INFO(fmt,...) YAZPGP_LOGGER_LOG(YAZPGP_LOGGER_OUTPUT_STDOUT, YAZPGP_LOGGER_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define YAZPGP_LOG_WARN(fmt,...) YAZPGP_LOGGER_LOG(YAZPGP_LOGGER_OUTPUT_STDOUT, YAZPGP_LOGGER_LEVEL_WARNING, fmt, ##__VA_ARGS__)
#define YAZPGP_LOG_ERROR(fmt,...) YAZPGP_LOGGER_LOG(YAZPGP_LOGGER_OUTPUT_STDERR, YAZPGP_LOGGER_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define YAZPGP_LOG_FATAL(fmt,...) do \
{ \
    YAZPGP_LOGGER_LOG(YAZPGP_LOGGER_OUTPUT_STDERR, YAZPGP_LOGGER_LEVEL_FATAL, fmt, ##__VA_ARGS__); \
    exit(1); \
}while(0)


#define YAZPGP_LOG_FATAL_IF(condition, fmt, ...) do { \
    if (condition) { \
        YAZPGP_LOG_FATAL(fmt, ##__VA_ARGS__); \
    } \
    } while (0)

#define YAZPGP_LOG_ERROR_IF(condition, fmt, ...) do { \
    if (condition) { \
        YAZPGP_LOG_ERROR(fmt, ##__VA_ARGS__); \
    } \
    } while (0)

#define YAZPGP_LOG_WARN_IF(condition, fmt, ...) do { \
    if (condition) { \
        YAZPGP_LOG_WARN(fmt, ##__VA_ARGS__); \
    } \
    } while (0)

#define YAZPGP_LOG_INFO_IF(condition, fmt, ...) do { \
    if (condition) { \
        YAZPGP_LOG_INFO(fmt, ##__VA_ARGS__); \
    } \
    } while (0)

#define YAZPGP_LOG_DEBUG_IF(condition, fmt, ...) do { \
    if (condition) { \
        YAZPGP_LOG_DEBUG(fmt, ##__VA_ARGS__); \
    } \
    } while (0)


#define YAZPGP_WATCH(variable) do { \
        std::cout << #variable " = " << variable << " (" __FILE__ ", " __LINE__ ")" << std::endl; \
} while (0)
