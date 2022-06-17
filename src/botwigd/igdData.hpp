#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "igdUnloadCheckData.hpp"

namespace sead {
struct TextWriter;
}

namespace ksys {
namespace act {
struct BaseProc;
}
namespace igd {

class DebugData {
public:
    DebugData() = default;
    ~DebugData() = default;
    static DebugData* Instance();
    void Render(sead::TextWriter* /* textWriter */);
    void IncFrameCounter() { mDebugFrameCounter++; }
    u32 GetFrameCounter() { return mDebugFrameCounter; }
    s32 mUnloadCheckFrame = -1;

    act::BaseProc* mLastElevator = nullptr;
    UnloadCheckData mLastElevatorUCD;
    act::BaseProc* mLastElevatorSP = nullptr;
    UnloadCheckData mLastElevatorSPUCD;
    // DgnObj_DungeonEntrance_A_01
    act::BaseProc* mLastEntrance = nullptr;
    UnloadCheckData mLastEntranceUCD;
    act::BaseProc* mLastEntranceSP = nullptr;
    UnloadCheckData mLastEntranceSPUCD;
    act::BaseProc* mLastEntranceDLC = nullptr;
    UnloadCheckData mLastEntranceDLCUCD;
    sead::Vector3f mPlayerPos;

private:
    void DebugData::RenderActor(sead::TextWriter* textWriter, const char* name, act::Actor* actor,
                                UnloadCheckData& unloadCheckData);
    u32 mDebugFrameCounter = 0;
};
}  // namespace igd
}  // namespace ksys
