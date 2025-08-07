//
// Created by brosobad on 09/07/2025.
//

#include "MemoryAllocator.h"

MemoryAllocator::MemoryAllocator(size_t memorySize, void* address) :
mStartAddress(address), mMemorySize(memorySize), mUsedMemory(0), mAllocationCount(0)
{

}

uint8_t MemoryAllocator::GetAddressAdjustment(const void* address, uint8_t alignment) {
    uintptr_t remainder = reinterpret_cast<uintptr_t>(address) & (alignment - 1);
    if (remainder == 0) {
        return 0;
    }
    return alignment - remainder;
}

uint8_t MemoryAllocator::GetAddressAdjustment(const void* address, uint8_t alignment, uint8_t extraMemory) {
    uint8_t padding = GetAddressAdjustment(address, alignment);
    if (padding < extraMemory) {
        uint8_t remainPadding = extraMemory - padding;
        if ((remainPadding & (alignment - 1)) != 0) {
            padding += alignment * (1 + (remainPadding / alignment));
        } else {
            padding += alignment * (remainPadding / alignment);
        }
    }
    return padding;
}

size_t MemoryAllocator::AlignForward(size_t memorySize, uint8_t alignment) {
    uintptr_t remainder = memorySize & (alignment - 1);
    if (remainder != 0) {
        memorySize += alignment - remainder;
    }
    return memorySize;
}

bool MemoryAllocator::IsPowerOfTwo(uint8_t alignment) {
    return (alignment & (alignment - 1)) == 0;
}

