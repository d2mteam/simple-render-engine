//
// Created by brosobad on 12/07/2025.
//

#include "MemoryStackManager.h"
#include <cstdlib>

MemoryStackManager::MemoryStackManager(size_t bufferSize) : mBufferSize(bufferSize) {
      mStartAddress = malloc(bufferSize);
      mStackAllocator = new StackAllocator(bufferSize, mStartAddress);
}

MemoryStackManager::~MemoryStackManager() {
    free(mStartAddress);
    delete mStackAllocator;
}


void MemoryStackManager::Clear() const {
    mStackAllocator->Clear();
}

// void MemoryStackManager::Free(void* memory) const {
//     if (memory != nullptr) {
//         return;
//     }
//     mStackAllocator->Free(memory);
// }


void MemoryStackManager::Pop() const {
    mStackAllocator->Pop();
}

void* MemoryStackManager::Allocate(size_t memorySize, uint8_t alignment) {
    return mStackAllocator->Allocate(memorySize, alignment);
}


void MemoryStackManager::DetectMemoryLeaks() {
}