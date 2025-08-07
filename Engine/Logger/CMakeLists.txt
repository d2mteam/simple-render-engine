add_library(Logger STATIC Logger.cpp)

find_package(spdlog CONFIG REQUIRED)


target_include_directories(Logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Logger
        PUBLIC
        spdlog::spdlog_header_only
)
