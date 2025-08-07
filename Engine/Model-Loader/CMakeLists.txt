add_library(Model-Loader STATIC ObjLoader.cpp ImageLoader.cpp)
target_link_libraries(Model-Loader PRIVATE Logger)
target_include_directories(Model-Loader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
