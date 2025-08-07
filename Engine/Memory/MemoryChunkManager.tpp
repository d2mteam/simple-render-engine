//
// Created by brosobad on 13/07/2025.
//
#ifndef MEMORY_CHUNK_MANAGER_TPP
#define MEMORY_CHUNK_MANAGER_TPP

#include "MemoryChunkManager.h"

template<typename T, size_t MAX_OBJECT_PER_CHUNK>
MemoryChunkManager<T, MAX_OBJECT_PER_CHUNK>::MemoryChunkManager(const char* mUsage) : mUsage(mUsage) {}

template<typename T, size_t MAX_OBJECT_PER_CHUNK>
MemoryChunkManager<T, MAX_OBJECT_PER_CHUNK>::~MemoryChunkManager() = default;

template<typename T, size_t MAX_OBJECT_PER_CHUNK>
template<typename... Args>
T *MemoryChunkManager<T, MAX_OBJECT_PER_CHUNK>::NewObject(Args &&...args) {
    for (MemoryChunk &chunk: mChunks) {
        if (chunk.allocator->HasSlot()) {
            return new (chunk.allocator->Allocate())T(std::forward<Args>(args)...);
        }
    }

    size_t memorySize = sizeof(T) * MAX_OBJECT_PER_CHUNK + alignof(T);
    void* address = malloc(memorySize);
    mStartChunkAddressList.push_back(address);
    MemoryChunk chunk = MemoryChunk(new PoolAllocator(memorySize, address, sizeof(T), alignof(T)));
    mChunks.push_back(chunk);
    return new (chunk.allocator->Allocate())T(std::forward<Args>(args)...);

}

template<typename T, std::size_t MAX_OBJECT_PER_CHUNK>
void MemoryChunkManager<T, MAX_OBJECT_PER_CHUNK>::FreeObject(void *memory) {
    if (memory == nullptr) {
        return;
    }

    for (MemoryChunk &chunk: mChunks) {
        if (chunk.allocator->ContainsSlot(memory)) {
            chunk.allocator->Free(memory);
        }
    }
}

template<typename T, std::size_t MAX_OBJECT_PER_CHUNK>
void MemoryChunkManager<T, MAX_OBJECT_PER_CHUNK>::Reset() {
    for (MemoryChunk &chunk: mChunks) {
        chunk.allocator->Clear();
    }
    mChunks.clear();
    for (void* address : mStartChunkAddressList) {
        free(address);
    }
}

#endif // MEMORY_CHUNK_MANAGER_TPP