//
// Created by brosobad on 14/07/2025.
//

#ifndef MEMORY_LINEAR_MANAGER_H
#define MEMORY_LINEAR_MANAGER_H

#include "MemoryAllocator.h"
#include <cstdint>
#include <utility>


class MemoryLinearManager {
public:

    MemoryLinearManager(size_t bufferSize);
    ~MemoryLinearManager();

    void Clear() const;
    void* AllocatePerFrame(size_t memorySize, uint8_t alignment);
    void DetectMemoryLeaks();

    template<typename T, typename... Args>
    T* NewPerFrame(Args&&... args) {
        void* address = mLinearAllocator->Allocate(sizeof(T), alignof(T));
        return new (address)T(std::forward<Args>(args)...);
    }
private:
    LinearAllocator* mLinearAllocator;
    void* mStartAddress;
    size_t mBufferSize;
};

#endif //MEMORY_LINEAR_MANAGER_H
