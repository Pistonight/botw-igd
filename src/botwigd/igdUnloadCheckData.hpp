#pragma once

#include <basis/seadTypes.h>

namespace ksys::act {
struct Actor;
}

namespace ksys::igd {

class UnloadCheckData {
public:
    UnloadCheckData() = default;
    ~UnloadCheckData() = default;

    bool isValid() { return mActorUnsafe != nullptr; }
    void Clear() { mActorUnsafe = nullptr; }
    void SetData(act::Actor* actor, u32 checkTime, bool shouldUnload, u32 reason) {
        mActorUnsafe = actor;
        mCheckTime = checkTime;
        mShouldUnload = shouldUnload;
        mReason = reason;
    }

    act::Actor* GetUnsafeActor() { return mActorUnsafe; }
    u32 GetCheckTime() { return mCheckTime; }
    bool ShouldUnload() { return mShouldUnload; }
    u32 GetReason() { return mReason; }

private:
    act::Actor* mActorUnsafe = nullptr;
    u32 mCheckTime = -1;
    bool mShouldUnload = false;
    u32 mReason = 0;
};

}  // namespace ksys::igd
