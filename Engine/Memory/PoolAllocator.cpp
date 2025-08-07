//
// Created by brosobad on 12/07/2025.
//

#include "Logger.h"
#include "MemoryAllocator.h"

PoolAllocator::PoolAllocator(size_t memorySize, void *address, size_t chunkSize, uint8_t chunkAlignment) :
    MemoryAllocator(memorySize, address), mChunkSize(chunkSize), mChunkAlignment(chunkAlignment)
{
    union {
        void *asVoidPtrAddress;
        uintptr_t asUintPtrAddress;
    };

    asVoidPtrAddress = mStartAddress;
    asVoidPtrAddress = mStartAddress;
    size_t adjustedAddress = AlignForward(asUintPtrAddress, mChunkAlignment);
    mAddressOffset = adjustedAddress - asUintPtrAddress;
    mMemorySize -= mAddressOffset;

    mChunkSize = AlignForward(mChunkSize, mChunkAlignment);
    assert(mChunkSize >= sizeof(FreeNode));
    assert(mMemorySize >= mChunkSize);

    mFreeListHead = nullptr;
    this->Clear();
}

PoolAllocator::~PoolAllocator() = default;


void* PoolAllocator::Allocate() {
    FreeNode* node = mFreeListHead;
    assert(node != nullptr);
    mFreeListHead = node->Next;
    mUsedMemory += mChunkSize;
    mAllocationCount++;
    return node;
}

void PoolAllocator::Free(void* memory) {
    assert(memory != nullptr);
    assert(Contains(memory));

    FreeNode* node = reinterpret_cast<FreeNode*>(memory);
    node->Next = mFreeListHead;
    mFreeListHead = node;

    mUsedMemory -= mChunkSize;
    mAllocationCount--;
}

void PoolAllocator::Clear() {
    size_t numberOfChunks = mMemorySize / mChunkSize;

    union {
        void* asVoidPtrAddress;
        char* asCharPtrAddress;
    };

    asVoidPtrAddress = mStartAddress;
    for (size_t idx = 0; idx < numberOfChunks; idx++) {
        FreeNode* node = reinterpret_cast<FreeNode*>(asCharPtrAddress + mAddressOffset + idx * mChunkSize);
        node->Next = mFreeListHead;
        mFreeListHead = node;
    }

    mUsedMemory = 0;
    mAllocationCount = 0;
}

bool PoolAllocator::Contains(void *memory) const {
    union {
        void *asVoidPtrAddress;
        uintptr_t asUintPtrAddress;
    };
    asVoidPtrAddress = mStartAddress;

    uintptr_t numberOfChunks = mMemorySize / mChunkSize;
    uintptr_t uintPtrMemory = reinterpret_cast<uintptr_t>(memory);

    uintptr_t chunkStart = asUintPtrAddress + mAddressOffset;
    uintptr_t chunkEnd = chunkStart + (numberOfChunks - 1) * mChunkSize;
    return uintPtrMemory >= chunkStart && uintPtrMemory <= chunkEnd;
}
bool PoolAllocator::HasSlot() const {
    return (mFreeListHead != nullptr);
}
