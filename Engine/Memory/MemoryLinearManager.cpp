//
// Created by brosobad on 14/07/2025.
//

#include "MemoryLinearManager.h"
#include <cstdlib>

MemoryLinearManager::MemoryLinearManager(size_t bufferSize) : mBufferSize(bufferSize) {
    mStartAddress = malloc(bufferSize);
    mLinearAllocator = new LinearAllocator(mBufferSize, mStartAddress);
}

MemoryLinearManager::~MemoryLinearManager() {
    free(mStartAddress);
}
void MemoryLinearManager::Clear() const {
    mLinearAllocator->Clear();
}

void *MemoryLinearManager::AllocatePerFrame(size_t memorySize, uint8_t alignment) {
    return mLinearAllocator->Allocate(memorySize, alignment);
}

void MemoryLinearManager::DetectMemoryLeaks() {

}