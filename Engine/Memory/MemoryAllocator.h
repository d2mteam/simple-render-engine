//
// Created by brosobad on 09/07/2025.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstdint>
#include <cstddef>

const uint8_t INVALID_MEMORY_SIZE = 0;
const uint8_t MAX_ALLOWED_ALIGNMENT = 128;

class MemoryAllocator {
public:
    MemoryAllocator(size_t memorySize, void* address);
    virtual ~MemoryAllocator() = default;
    void* GetStartAddress() const { return mStartAddress; }
protected:
    uint8_t GetAddressAdjustment(const void* address, uint8_t alignment);
    uint8_t GetAddressAdjustment(const void* address, uint8_t alignment, uint8_t extraMemory);
    size_t AlignForward(size_t memorySize, uint8_t alignment);
    bool IsPowerOfTwo(uint8_t alignment);
protected:
    void* mStartAddress;
    size_t mMemorySize;
    size_t mUsedMemory;
    size_t mAllocationCount;
};


class LinearAllocator : public MemoryAllocator {
public:
    LinearAllocator(size_t memorySize, void* address);
    ~LinearAllocator() override;

    void* Allocate(size_t memorySize, uint8_t alignment);
    void Clear();
};

class StackAllocator : public MemoryAllocator {
    struct Header {
        uint8_t Padding;
        size_t Size;
    };
public:
    StackAllocator(size_t memorySize, void* address);
    ~StackAllocator() override;
    void* Allocate(size_t memorySize, uint8_t alignment);
    // void Free(void* memory);
    void Pop();
    void Clear();
private:
    void* mCurrentTop;
};

class PoolAllocator : public MemoryAllocator {
    struct FreeNode {
        FreeNode* Next;
    };
public:
    PoolAllocator(size_t memorySize, void* address, size_t chunkSize, uint8_t chunkAlignment);
    ~PoolAllocator() override;

    void* Allocate();
    void Free(void* memory);
    void Clear();
    bool Contains(void* memory) const;
    bool HasSlot() const;
private:
    size_t mChunkSize;
    uint8_t mChunkAlignment;
    FreeNode* mFreeListHead;
    uint8_t mAddressOffset;
};

#endif //MEMORY_ALLOCATOR_H
