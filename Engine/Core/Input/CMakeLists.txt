add_library(Input STATIC KeyCode.h Input.h GLFWInput.cpp)

target_include_directories(Input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Input
        PUBLIC
)
