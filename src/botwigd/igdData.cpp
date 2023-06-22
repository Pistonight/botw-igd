#include "igdData.hpp"
#include <KingSystem/ActorSystem/actActor.h>
#include <KingSystem/ActorSystem/actBaseProc.h>
#include <gfx/seadTextWriter.h>
#include <math/seadMathCalcCommon.h>
#include <math/seadVector.h>

namespace ksys::igd {
static DebugData sDebugDataInstance;
DebugData* DebugData::Instance() {
    return &sDebugDataInstance;
}

void DebugData::Render(sead::TextWriter* textWriter) {
    textWriter->printf("sDebugData 0x%08x\n", this);
    textWriter->printf("  mPlayerPos (%.4f, %.4f, %.4f)\n", mPlayerPos.x, mPlayerPos.y,
                       mPlayerPos.z);
    textWriter->printf("  mDebugFrameCounter %d\n", mDebugFrameCounter);
    if (mUnloadCheckFrame != -1) {
        textWriter->printf("  mUnloadCheckFrame %02d\n", mUnloadCheckFrame);
    }

    RenderActor(textWriter, "Elevator", reinterpret_cast<act::Actor*>(mLastElevator),
                mLastElevatorUCD);
    RenderActor(textWriter, "Entrance", reinterpret_cast<act::Actor*>(mLastEntrance),
                mLastEntranceUCD);
    // RenderActor(textWriter, "ElevatorSP", reinterpret_cast<act::Actor*>(mLastElevatorSP),
    //             mLastElevatorSPUCD);
    // RenderActor(textWriter, "EntranceSP", reinterpret_cast<act::Actor*>(mLastEntranceSP),
    //             mLastEntranceSPUCD);
    // RenderActor(textWriter, "EntranceDLC", reinterpret_cast<act::Actor*>(mLastEntranceDLC),
    //             mLastEntranceDLCUCD);
    float targetX = 2040.49;
    float targetZ = 972.22;
    sead::Vector3f laknaRokee {targetX, mPlayerPos.y, targetZ};
    textWriter->printf("  distance to lakna %f\n", (laknaRokee-mPlayerPos).length());

    // if (mLastElevator) {
    //     textWriter->printf("  mLastElevator 0x%08x\n", mLastElevator);
    //     // ID
    //     textWriter->printf("    ID 0x%08x (%02d)\n", mLastElevator->getId(),
    //                        mLastElevator->getId() % 30);
    //     textWriter->printf("    Name %s\n", mLastElevator->getName().cstr());
    //     textWriter->printf("    State %d\n", mLastElevator->getState());
    //     // actor stuff
    //     act::Actor* elevatorActor = reinterpret_cast<act::Actor*>(mLastElevator);
    //     textWriter->printf("    Delete Distance %.4f\n", elevatorActor->getDeleteDistance());
    //     auto mtx = elevatorActor->getMtx();
    //     // textWriter->printf("    mMtx %.4f %.4f %.4f %.4f\n", mtx.m[0][0], mtx.m[0][1],
    //     mtx.m[0][2], mtx.m[0][3]);
    //     // textWriter->printf("         %.4f %.4f %.4f %.4f\n", mtx.m[1][0], mtx.m[1][1],
    //     mtx.m[1][2], mtx.m[1][3]);
    //     // textWriter->printf("         %.4f %.4f %.4f %.4f\n", mtx.m[2][0], mtx.m[2][1],
    //     mtx.m[2][2], mtx.m[2][3]); textWriter->printf("    Coord (mMtx) %.4f %.4f %.4f\n",
    //     mtx.m[0][3], mtx.m[1][3], mtx.m[2][3]);
    // }

    // if(mShouldUnloadLastElevator){
    //     textWriter->printf("  mSULElevator 0x%08x\n", mShouldUnloadLastElevator);
    //     textWriter->printf("  mSULCheckTime %d\n", mShouldUnloadLastCheckTime);
    //     textWriter->printf("  mSULCheckResult %s\n", mShouldUnloadLastCheckResult ? "Unload" :
    //     "Stay Loaded"); textWriter->printf("  mSULReason 0x%x\n", mShouldUnloadLastReason);
    // }

    // if(mLastDungeonEntranceFar){
    //     textWriter->printf("  mLastDungeonEntranceFar 0x%08x\n", mLastDungeonEntranceFar);
    //     // ID
    //     textWriter->printf("    ID 0x%08x (%02d)\n", mLastDungeonEntranceFar->getId(),
    //                        mLastDungeonEntranceFar->getId() % 30);
    //     textWriter->printf("    Name %s\n", mLastDungeonEntranceFar->getName().cstr());
    //     textWriter->printf("    State %d\n", mLastDungeonEntranceFar->getState());
    //     // actor stuff
    //     act::Actor* entranceActor = reinterpret_cast<act::Actor*>(mLastDungeonEntranceFar);
    //     textWriter->printf("    Delete Distance %.4f\n", entranceActor->getDeleteDistance());
    //     auto mtx = entranceActor->getMtx();
    //     // textWriter->printf("    mMtx %.4f %.4f %.4f %.4f\n", mtx.m[0][0], mtx.m[0][1],
    //     mtx.m[0][2], mtx.m[0][3]);
    //     // textWriter->printf("         %.4f %.4f %.4f %.4f\n", mtx.m[1][0], mtx.m[1][1],
    //     mtx.m[1][2], mtx.m[1][3]);
    //     // textWriter->printf("         %.4f %.4f %.4f %.4f\n", mtx.m[2][0], mtx.m[2][1],
    //     mtx.m[2][2], mtx.m[2][3]); textWriter->printf("    Coord (mMtx) %.4f %.4f %.4f\n",
    //     mtx.m[0][3], mtx.m[1][3], mtx.m[2][3]); textWriter->printf("    mLLDM %.4f\n",
    //     entranceActor->getLodLoadDistanceMultiplier());
    // }

    // if(mShouldUnloadLastDungeonEntranceFar){
    //     textWriter->printf("  mSULDungeonEntranceFar 0x%08x\n",
    //     mShouldUnloadLastDungeonEntranceFar); textWriter->printf("  mSULCheckTimeDEF %d\n",
    //     mShouldUnloadLastCheckTimeDungeonEntranceFar); textWriter->printf("  mSULCheckResultDEF
    //     %s\n", mShouldUnloadLastCheckResultDungeonEntranceFar ? "Unload" : "Stay Loaded");
    //     textWriter->printf("  mSULReasonDEF 0x%x\n", mShouldUnloadLastReasonDungeonEntranceFar);
    // }

    // if (mLastElevatorSP) {
    //     textWriter->printf("  mLastElevatorSP 0x%08x\n", mLastElevatorSP);
    //     // ID
    //     textWriter->printf("    ID 0x%08x (%02d)\n", mLastElevatorSP->getId(),
    //                        mLastElevatorSP->getId() % 30);
    //     textWriter->printf("    Name %s\n", mLastElevatorSP->getName().cstr());
    //     textWriter->printf("    State %d\n", mLastElevatorSP->getState());
    // }
}

void DebugData::RenderActor(sead::TextWriter* textWriter, const char* name, act::Actor* actor,
                            UnloadCheckData& unloadCheckData) {
    textWriter->printf("  <0x%08x> %s\n", actor, name);
    if (actor) {
        textWriter->printf("    0x%08x (%02d) %s\n", actor->getId(), actor->getId() % 30,
                           actor->getName().cstr());
        textWriter->printf("    Delete Distance %.4f\n", actor->getDeleteDistance());
        auto mtx = actor->getMtx();
        sead::Vector3f actorPos(mtx.m[0][3], mtx.m[1][3], mtx.m[2][3]);
        textWriter->printf("    Coord (mMtx) %.4f %.4f %.4f\n", actorPos.x, actorPos.y, actorPos.z);
        f32 distanceToPlayer = (actorPos - mPlayerPos).length();
        f32 dx = actorPos.x - mPlayerPos.x;
        f32 dz = actorPos.z - mPlayerPos.z;
        f32 distance2DToPlayer = sead::MathCalcCommon<f32>::sqrt(dx * dx + dz * dz);
        textWriter->printf("    To Player %.4f (2D=%.4f)\n", distanceToPlayer, distance2DToPlayer);
    }
    auto unsafeActor = unloadCheckData.GetUnsafeActor();
    auto checkTime = unloadCheckData.GetCheckTime();
    auto shouldUnload = unloadCheckData.ShouldUnload();
    auto reason = unloadCheckData.GetReason();
    if (unsafeActor) {
        if (unsafeActor != actor) {
            textWriter->printf("    [Previous UCD]<0x%08x> %d:%s (%x)\n", unsafeActor, checkTime,
                               shouldUnload ? "Unload" : "Stay Loaded", reason);
        } else {
            textWriter->printf("    [UCD] %d:%s (%x)\n", checkTime,
                               shouldUnload ? "Unload" : "Stay Loaded", reason);
        }
    } else {
        textWriter->printf("    [No Unload Check Data]\n");
    }
}

}  // namespace ksys::igd
