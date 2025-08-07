//
// Created by brosobad on 10/07/2025.
//

#include "Logger.h"
#include "MemoryAllocator.h"

LinearAllocator::LinearAllocator(size_t memorySize, void *address) : MemoryAllocator(memorySize, address) {
}

LinearAllocator::~LinearAllocator() = default;

void* LinearAllocator::Allocate(size_t memorySize, uint8_t alignment) {
    assert(memorySize > INVALID_MEMORY_SIZE);
    assert(alignment < MAX_ALLOWED_ALIGNMENT);

    union {
        void *asVoidPtrAddress;
        uintptr_t asUintPtrAddress;
    };

    asVoidPtrAddress = mStartAddress;
    asUintPtrAddress += mUsedMemory;


    uintptr_t adjustment = GetAddressAdjustment(asVoidPtrAddress, alignment);

    if (mUsedMemory + memorySize + adjustment > mMemorySize) {
        CORE_LOG_WARN("LinearAllocator is full, can not allocate for new data with size {0}", memorySize);
        return nullptr;
    }

    asUintPtrAddress += adjustment;
    mUsedMemory += memorySize + adjustment;
    mAllocationCount += 1;
    CORE_LOG_INFO("linear pointer generated {0} {1}, start {2}",asUintPtrAddress, asVoidPtrAddress,mStartAddress);
    return reinterpret_cast<void*>(asUintPtrAddress);;
}

void LinearAllocator::Clear() {
    mUsedMemory = 0;
    mAllocationCount = 0;
}