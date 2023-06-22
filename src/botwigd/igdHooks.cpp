#include "igdHooks.hpp"
#include <KingSystem/ActorSystem/actActor.h>
#include <skyline/inlinehook/And64InlineHook.hpp>
#include "igdData.hpp"

namespace ksys::igd::hook {
static bool (*sShouldUnloadBcDistanceOriginal)(act::Actor*, unsigned int*);
void Init() {
    A64HookFunction(reinterpret_cast<void*>(act__Actor__shouldUnloadBecauseOfDistance),
                    reinterpret_cast<void*>(ShouldUnloadBcDistance),
                    (void**)&sShouldUnloadBcDistanceOriginal);
}

bool ShouldUnloadBcDistance(act::Actor* actorThis, unsigned int* out_reason) {
    auto debugData = DebugData::Instance();
    if (debugData->mLastElevator == actorThis) {
        auto checkTime = debugData->GetFrameCounter();
        u32 reason = 0;
        bool result = sShouldUnloadBcDistanceOriginal(actorThis, &reason);
        debugData->mLastElevatorUCD.SetData(actorThis, checkTime, result, reason);

        if (out_reason) {
            *out_reason = 0;//reason;  april fools
        }
        return false; // result;  april fools
    } else if (debugData->mLastEntrance == actorThis) {
        auto checkTime = debugData->GetFrameCounter();
        u32 reason = 0;
        bool result = sShouldUnloadBcDistanceOriginal(actorThis, &reason);
        debugData->mLastEntranceUCD.SetData(actorThis, checkTime, result, reason);

        if (out_reason) {
            *out_reason = reason;
        }
        return result;
    } else if (debugData->mLastElevatorSP == actorThis) {
        auto checkTime = debugData->GetFrameCounter();
        u32 reason = 0;
        bool result = sShouldUnloadBcDistanceOriginal(actorThis, &reason);
        debugData->mLastElevatorSPUCD.SetData(actorThis, checkTime, result, reason);

        if (out_reason) {
            *out_reason = reason;
        }
        return result;
    } else if (debugData->mLastEntranceSP == actorThis) {
        auto checkTime = debugData->GetFrameCounter();
        u32 reason = 0;
        bool result = sShouldUnloadBcDistanceOriginal(actorThis, &reason);
        debugData->mLastEntranceSPUCD.SetData(actorThis, checkTime, result, reason);

        if (out_reason) {
            *out_reason = reason;
        }
        return result;
    } else if (debugData->mLastEntranceDLC == actorThis) {
        auto checkTime = debugData->GetFrameCounter();
        u32 reason = 0;
        bool result = sShouldUnloadBcDistanceOriginal(actorThis, &reason);
        debugData->mLastEntranceDLCUCD.SetData(actorThis, checkTime, result, reason);

        if (out_reason) {
            *out_reason = reason;
        }
        return result;
    } else {
        return sShouldUnloadBcDistanceOriginal(actorThis, out_reason);
    }
}

}  // namespace ksys::igd::hook
