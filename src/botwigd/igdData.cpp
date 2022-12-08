#include "igdData.hpp"
#include <KingSystem/ActorSystem/actActor.h>
#include <KingSystem/ActorSystem/actActorSystem.h>
#include <KingSystem/ActorSystem/actBaseProc.h>
#include <KingSystem/ActorSystem/actBaseProcMgr.h>
#include <gfx/seadTextWriter.h>
#include <math/seadMathCalcCommon.h>
#include <math/seadVector.h>

namespace ksys::igd {
static DebugData sDebugDataInstance;
DebugData* DebugData::Instance() {
    return &sDebugDataInstance;
}

void DebugData::Render(sead::TextWriter* textWriter) {
    act::ActorSystem* actSys = act::ActorSystem::instance();
    if (actSys) {
        textWriter->printf("mEmergencyHeap 0x%08x\n", actSys->getEmergencyHeap());
        const char* mc0 = reinterpret_cast<const char*>(actSys->mc0());
        if(mc0){
            char* mGameROMPlayerMaybe = const_cast<char*>(mc0) - 0xC38;
            textWriter->printf("mGameROMPlayerMaybe 0x%016x\n", mGameROMPlayerMaybe);
            char* pWeapons = mGameROMPlayerMaybe + 0xB90 + 0x8;
            char* ppBaseproc = pWeapons + 0x58;
            auto ppBaseProcCast = reinterpret_cast<act::BaseProc**>(ppBaseproc);
            if(ppBaseProcCast){
                auto pBaseProc = *ppBaseProcCast;
                if(pBaseProc){
                    textWriter->printf("    0x%08x (%02d) %s\n", pBaseProc->getId(), pBaseProc->getId() % 30,
                            pBaseProc->getName().cstr());
                }
            }
            
             //act::BaseProc* pBaseProc = reinterpret_cast<act::BaseProc*>(mGameROMPlayerMaybe);
            
        }
        
    }
    textWriter->printf("sDebugData 0x%08x\n", this);
    textWriter->printf("  mPlayerPos (%.4f, %.4f, %.4f)\n", mPlayerPos.x, mPlayerPos.y,
                       mPlayerPos.z);
    textWriter->printf("  mPlayerMoveSpeed %.4f\n", mPlayerMoveSpeed);
    textWriter->printf("  mDebugFrameCounter %d\n", mDebugFrameCounter);
    if (mUnloadCheckFrame != -1) {
        textWriter->printf("  mUnloadCheckFrame %02d\n", mUnloadCheckFrame);
    }

    // RenderActor(textWriter, "Elevator", reinterpret_cast<act::Actor*>(mLastElevator),
    //             mLastElevatorUCD);
    // RenderActor(textWriter, "Entrance", reinterpret_cast<act::Actor*>(mLastEntrance),
    //             mLastEntranceUCD);
    // RenderActor(textWriter, "ElevatorSP", reinterpret_cast<act::Actor*>(mLastElevatorSP),
    //             mLastElevatorSPUCD);
    // RenderActor(textWriter, "EntranceSP", reinterpret_cast<act::Actor*>(mLastEntranceSP),
    //             mLastEntranceSPUCD);
    // RenderActor(textWriter, "EntranceDLC", reinterpret_cast<act::Actor*>(mLastEntranceDLC),
    //             mLastEntranceDLCUCD);
    RenderActor(textWriter, "Kak007", reinterpret_cast<act::Actor*>(mKak007),
                mKak007UCD);
   
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
