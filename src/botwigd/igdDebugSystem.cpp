#include <heap/seadExpHeap.h>
#include <heap/seadHeapMgr.h>
#include <math/seadVector.h>

#include <KingSystem/ActorSystem/actActorSystem.h>
#include <KingSystem/ActorSystem/actBaseProcMgr.h>
#include <KingSystem/System/SystemTimers.h>

#include "igdData.hpp"
#include "igdDebugSystem.hpp"
#include "igdHooks.hpp"

namespace ksys::igd {
// Debug Data

// static s32 sUnloadCheckFrame;
// static act::BaseProc* sElevators[137];
// static s32 sElevatorsLength;
// static act::BaseProc* sLastElevator;
// static act::BaseProc* sLastElevatorSP;
static sead::ExpHeap* sSkylineHeap = nullptr;

void Init() {
    // Calling this so the function is not removed by linker
    RenderDebugScreen(nullptr);
    ComputeDebugData();
    hook::Init();
    // sUnloadCheckFrame = -1;
    // sElevatorsLength = 0;
    // sLastElevator = nullptr;
    // sLastElevatorSP = nullptr;
    // Create a heap
    // auto rootHeap = sead::HeapMgr::getRootHeaps()[0];
    // sSkylineHeap = sead::ExpHeap::create(33554432u /*32MB*/, "SkylineHeap", rootHeap, 0x1000,
    // sead::ExpHeap::cHeapDirection_Forward, false);
}

void ComputeDebugData() {
    auto debugData = DebugData::Instance();
    debugData->IncFrameCounter();
    SystemTimers* systemTimers = SystemTimers::instance();
    if (systemTimers) {
        debugData->mUnloadCheckFrame = systemTimers->mFrameCounter % 30;
        act::BaseProcMgr* baseProcMgr = act::BaseProcMgr::instance();

        if (baseProcMgr) {
            // ElevatorProcessorFunc processor {sElevators, &sElevatorsLength};
            // sead::Delegate1<ElevatorProcessorFunc, act::BaseProc*> delegate {&processor,
            // &ElevatorProcessorFunc::invoke}; sElevatorsLength = 0;
            // baseProcMgr->forEachProc("DgnObj_EntranceElevator_A_01", delegate, {});
            debugData->mLastElevator = baseProcMgr->getProc("DgnObj_EntranceElevator_A_01", {});
            debugData->mLastElevatorSP = baseProcMgr->getProc("DgnObj_EntranceElevatorSP", {});
            debugData->mLastEntrance = baseProcMgr->getProc("DgnObj_DungeonEntrance_A_01", {});
            debugData->mLastEntranceSP = baseProcMgr->getProc("DgnObj_DungeonEntranceSP", {});
            debugData->mLastEntranceDLC =
                baseProcMgr->getProc("DgnObj_DLC_ChampionsDungeonEntrance_A_01", {});
            // //baseProcMgr->forEachProc("DgnObj_EntranceElevatorSP", processor, {});
        } else {
            debugData->mLastElevator = nullptr;
            debugData->mLastElevatorUCD.Clear();
            debugData->mLastEntrance = nullptr;
            debugData->mLastEntranceUCD.Clear();
            debugData->mLastElevatorSP = nullptr;
            debugData->mLastElevatorSPUCD.Clear();
            debugData->mLastEntranceSP = nullptr;
            debugData->mLastEntranceSPUCD.Clear();
            debugData->mLastEntranceDLC = nullptr;
            debugData->mLastEntranceDLCUCD.Clear();
        }
    } else {
        debugData->mUnloadCheckFrame = -1;
        debugData->mLastElevator = nullptr;
        debugData->mLastElevatorUCD.Clear();
        debugData->mLastEntrance = nullptr;
        debugData->mLastEntranceUCD.Clear();
        debugData->mLastElevatorSP = nullptr;
        debugData->mLastElevatorSPUCD.Clear();
        debugData->mLastEntranceSP = nullptr;
        debugData->mLastEntranceSPUCD.Clear();
        debugData->mLastEntranceDLC = nullptr;
        debugData->mLastEntranceDLCUCD.Clear();
    }

    act::ActorSystem* actSys = act::ActorSystem::instance();
    if (actSys) {
        debugData->mPlayerPos = actSys->getPlayerPos();
    }
}

void RenderDebugScreen(sead::TextWriter* textWriter) {
    if (!textWriter) {
        // Dummy check so we could call this function in skyline so that the linker does not
        // optimize it out
        return;
    }

    if (sSkylineHeap) {
        textWriter->printf("In Game Debugger v0.0.0 Heap 0x%08x (%08x/%08x Free)\n", sSkylineHeap,
                           sSkylineHeap->getFreeSize(), sSkylineHeap->getSize());
    } else {
        textWriter->printf("In Game Debugger v0.0.0\n");
    }
    // Print empty stuff to not block UI
    textWriter->printf("              ");
    for (int i = 0; i < 5; i++) {
        textWriter->printf("\n");
    }

    SystemTimers* systemTimers = SystemTimers::instance();
    textWriter->printf("SystemTimers 0x%08x\n", systemTimers);
    if (systemTimers) {
        textWriter->printf("  mFrameCounter     %d\n", systemTimers->mFrameCounter);
        if ((int)systemTimers->mFrameCounter != systemTimers->mFrameCounterB) {
            // They are usually equal, but if not, print B
            textWriter->printf("  mFrameCounterB    %d\n", systemTimers->mFrameCounterB);
        }
        // textWriter->printf("  mFrameCounter2    %d\n", systemTimers->mFrameCounter2);
        // if(systemTimers->mFrameCounter2 != systemTimers->mFrameCounterB2){
        //     // They are usually equal, but if not, print B2
        //     textWriter->printf("  mFrameCounterB2   %d\n", systemTimers->mFrameCounterB2);
        // }
        // textWriter->printf("  mvfrTimer     %f\n", systemTimers->mVfrTimer);
        // if(systemTimers->mVfrTimer != systemTimers->mVfrTimer2){
        //     // They are usually equal, but if not, print 2
        //     textWriter->printf("  mvfrTimer2    %f\n", systemTimers->mVfrTimer2);
        // }
    }
    // if (sUnloadCheckFrame != -1) {
    //     textWriter->printf("sUnloadCheckFrame %02d\n", sUnloadCheckFrame);
    // }

    act::BaseProcMgr* baseProcMgr = act::BaseProcMgr::instance();
    textWriter->printf("act::BaseProcMgr 0x%08x\n", baseProcMgr);
    act::ActorSystem* actSys = act::ActorSystem::instance();
    textWriter->printf("act::ActorSystem 0x%08x\n", actSys);
    DebugData::Instance()->Render(textWriter);
}

}  // namespace ksys::igd
