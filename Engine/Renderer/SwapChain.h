//
// Created by brosobad on 18/07/2025.
//

#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H
#include <cstdint>

class SwapChain {
    virtual void Present() = 0;
    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual ~SwapChain() = default;
};
#endif //SWAP_CHAIN_H
