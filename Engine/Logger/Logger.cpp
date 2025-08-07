//
// Created by brosobad on 09/07/2025.
//

#include "Logger.h"

std::shared_ptr<spdlog::logger> Logger::sCoreLogger = nullptr;
std::shared_ptr<spdlog::logger> Logger::sClientLogger = nullptr;

void Logger::Init() {
    // Example: [22:40:00] [VIEngine::Logger:Init:11] [VIEngine] [Thread:1000] Logger works
    spdlog::set_pattern("%^[%H:%M:%S] [%!:%#] [%n] [Thread:%t] %v%$");

    sCoreLogger = spdlog::stdout_color_mt("Render-Engine");
    sCoreLogger->set_level(spdlog::level::trace);

    sClientLogger = spdlog::stdout_color_mt("Client");
    sClientLogger->set_level(spdlog::level::trace);
}
