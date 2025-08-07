//
// Created by brosobad on 10/07/2025.
//

#include "MemoryAllocator.h"
#include "Logger.h"

StackAllocator::StackAllocator(size_t memorySize, void *address) : MemoryAllocator(memorySize, address) {
    mCurrentTop = address;
}

StackAllocator::~StackAllocator() = default;

void* StackAllocator::Allocate(size_t memorySize, uint8_t alignment) {
    assert(memorySize > INVALID_MEMORY_SIZE);
    assert(alignment < MAX_ALLOWED_ALIGNMENT);

    union {
        void* asVoidPtrAddress;
        uintptr_t asUintPtrAddress;
        Header* asHeader;
    };

    asVoidPtrAddress = mStartAddress;
    asUintPtrAddress += mUsedMemory;

    uintptr_t padding = GetAddressAdjustment(asVoidPtrAddress, alignment, sizeof(Header));

    if (mUsedMemory + padding + memorySize > mMemorySize) {
        CORE_LOG_WARN("StackAllocator is full, can not allocate for new data with size {0}", memorySize);
        return nullptr;
    }

    mUsedMemory += memorySize + padding;
    asUintPtrAddress += padding;
    asUintPtrAddress -= sizeof(Header);
    asHeader->Padding = padding;
    asUintPtrAddress += sizeof(Header);
    mAllocationCount++;

    return reinterpret_cast<void*>(asUintPtrAddress);
}

// void StackAllocator::Free(void *memory) {
//     union {
//         void *asVoidPtrAddress;
//         uintptr_t asUintPtrAddress;
//         Header *asHeader;
//     };
//
//     asVoidPtrAddress = memory;
//     auto startMemory = reinterpret_cast<uintptr_t>(mStartAddress);
//
//     if (!(asUintPtrAddress >= startMemory && asUintPtrAddress < startMemory + mMemorySize)) {
//         CORE_LOG_ERROR("Free-out-of-bounds address {0}", memory);
//         return;
//     }
//
//     asUintPtrAddress -= sizeof(Header);
//     mUsedMemory = reinterpret_cast<uintptr_t>(memory) - startMemory - asHeader->Padding;
//     mAllocationCount--;
// }

void StackAllocator::Pop() {
    if (mCurrentTop == mStartAddress) {
        return;
    }
    union {
        void *asVoidPtrAddress;
        uintptr_t asUintPtrAddress;
        Header *asHeader;
    };

    asVoidPtrAddress = mCurrentTop;
    auto startMemory = reinterpret_cast<uintptr_t>(mStartAddress);
    if (!(asUintPtrAddress >= startMemory && asUintPtrAddress < startMemory + mMemorySize)) {
        CORE_LOG_ERROR("Free-out-of-bounds address {0}", mCurrentTop);
        return;
    }
    asUintPtrAddress -= sizeof(Header);
    mUsedMemory = reinterpret_cast<uintptr_t>(mCurrentTop) - startMemory - asHeader->Padding;

    uintptr_t newTop = asUintPtrAddress - asHeader->Padding;
    mCurrentTop = reinterpret_cast<uint8_t*>(newTop);

    mAllocationCount--;
}

void StackAllocator::Clear() {
    mUsedMemory = 0;
    mAllocationCount = 0;
}
