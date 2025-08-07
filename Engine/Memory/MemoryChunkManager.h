//
// Created by brosobad on 13/07/2025.
//

#ifndef MEMORY_CHUNK_MANAGER_H
#define MEMORY_CHUNK_MANAGER_H


#include <list>
#include "MemoryAllocator.h"

template<typename T, size_t MAX_OBJECT_PER_CHUNK>
class MemoryChunkManager {
public:
    MemoryChunkManager(const char* mUsage);
    ~MemoryChunkManager();

    template<typename... Args>
    T* NewObject(Args&&... args);

    void FreeObject(void* memory);
    void Reset();

private:
    struct MemoryChunk {
        PoolAllocator* allocator;
    };
    std::list<MemoryChunk> mChunks;
    std::vector<void*> mStartChunkAddressList;
    const char* mUsage;
};

#include "MemoryChunkManager.tpp"
#endif //MEMORY_CHUNK_MANAGER_H
