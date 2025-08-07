//
// Created by brosobad on 12/07/2025.
//

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "MemoryAllocator.h"
#include <utility>

class MemoryStackManager {
public:
    MemoryStackManager(size_t bufferSize);
    ~MemoryStackManager();
    void * Allocate(size_t memorySize, uint8_t alignment);
    // void Free(void *memory) const;
    void Pop() const;
    void Clear() const;
    void DetectMemoryLeaks();

    template<typename T, typename... Args>
    T* NewOnStack(Args &&...args) {
        void* address = Allocate(sizeof(T), alignof(T));
        return new (address)T(std::forward<Args>(args)...);
    }
private:
    StackAllocator* mStackAllocator;
    void* mStartAddress;
    size_t mBufferSize;
};



#endif //MEMORY_MANAGER_H
