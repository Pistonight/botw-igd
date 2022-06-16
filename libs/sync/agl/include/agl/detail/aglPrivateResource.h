#pragma once

#include <basis/seadTypes.h>
#include <heap/seadDisposer.h>
#include <hostio/seadHostIONode.h>
#include <prim/seadSafeString.h>
#include "agl/aglGPUMemBlock.hpp"

namespace agl::detail {

class PrivateResource : public sead::hostio::Node {
    SEAD_SINGLETON_DISPOSER(PrivateResource)

    PrivateResource();
    virtual ~PrivateResource();

public:
    sead::Heap* getWorkHeap() const { return mWorkHeap; }
    sead::Heap* getDebugHeap() const { return mDebugHeap; }

private:
    static void debugPrintFn(const sead::SafeString& str);

    sead::Heap* mWorkHeap{};
    void* _30{};
    u8 _38[0x58 - 0x38];
    u32 _58{};
    void* _60{};
    void* _68{};
    void* _70{};
    sead::Heap* mDebugHeap{};
    void* _80{};
    void* _88{};
    GPUMemBlock<u8> _90;
    void (*mDebugPrintFn)(const sead::SafeString& str) = debugPrintFn;
    u8 _d0[0x100 - 0xd0];
};

}  // namespace agl::detail
