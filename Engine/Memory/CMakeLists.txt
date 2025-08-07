add_library(Memory STATIC MemoryAllocator.cpp LinearAllocator.cpp StackAllocator.cpp PoolAllocator.cpp
        MemoryLinearManager.cpp MemoryStackManager.cpp MemoryChunkManager.tpp)
target_link_libraries(Memory PRIVATE Logger)
target_include_directories(Memory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
