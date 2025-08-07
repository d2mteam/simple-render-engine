//
// Created by brosobad on 09/07/2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>


class  Logger {
public:
    static void Init();
    static std::shared_ptr<spdlog::logger> GetCoreLogger() { return sCoreLogger; }
    static std::shared_ptr<spdlog::logger> GetClientLogger() { return sClientLogger; }
private:
    static std::shared_ptr<spdlog::logger> sCoreLogger;
    static std::shared_ptr<spdlog::logger> sClientLogger;
};

#endif //LOGGER_H

#define LOG_WITH_DETAILS(logger, level, ...) logger->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

#if !defined(NDEBUG)
    #define CORE_LOG_TRACE(...) LOG_WITH_DETAILS(Logger::GetCoreLogger(), spdlog::level::trace, __VA_ARGS__)
    #define CORE_LOG_DEBUG(...) LOG_WITH_DETAILS(Logger::GetCoreLogger(), spdlog::level::debug, __VA_ARGS__)
    #define CORE_LOG_INFO(...) LOG_WITH_DETAILS(Logger::GetCoreLogger(), spdlog::level::info, __VA_ARGS__)
    #define CORE_LOG_WARN(...) LOG_WITH_DETAILS(Logger::GetCoreLogger(), spdlog::level::warn, __VA_ARGS__)
    #define CORE_LOG_ERROR(...) LOG_WITH_DETAILS(Logger::GetCoreLogger(), spdlog::level::err, __VA_ARGS__)
    #define CORE_LOG_CRITICAL(...) LOG_WITH_DETAILS(Logger::GetCoreLogger(), spdlog::level::critical, __VA_ARGS__)

    #define LOG_TRACE(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::trace, __VA_ARGS__)
    #define LOG_DEBUG(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::debug, __VA_ARGS__)
    #define LOG_INFO(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::info, __VA_ARGS__)
    #define LOG_WARN(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::warn, __VA_ARGS__)
    #define LOG_ERROR(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::err, __VA_ARGS__)
    #define LOG_CRITICAL(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::critical, __VA_ARGS__)
#else
    #define CORE_LOG_TRACE(...)
    #define CORE_LOG_DEBUG(...)
    #define CORE_LOG_INFO(...)
    #define CORE_LOG_WARN(...)
    #define CORE_LOG_ERROR(...)
    #define CORE_LOG_CRITICAL(...)

    #define LOG_TRACE(...)
    #define LOG_DEBUG(...)
    #define LOG_INFO(...)
    #define LOG_WARN(...)
    #define LOG_ERROR(...)
    #define LOG_CRITICAL(...)
#endif