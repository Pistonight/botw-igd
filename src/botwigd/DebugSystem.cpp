#include <heap/seadExpHeap.h>
#include <heap/seadHeapMgr.h>
//#include <prim/seadDelegate.h>

#include <KingSystem/ActorSystem/actBaseProcMgr.h>
#include <KingSystem/System/SystemTimers.h>

#include "DebugSystem.hpp"

namespace ksys::igd {
// Debug Data
static s32 sUnloadCheckFrame;
// static act::BaseProc* sElevators[137];
// static s32 sElevatorsLength;
static act::BaseProc* sLastElevator;
static act::BaseProc* sLastElevatorSP;
static sead::ExpHeap* sSkylineHeap;

void Init() {
    // Calling this so the function is not removed by linker
    RenderDebugScreen(nullptr);
    ComputeDebugData();
    sUnloadCheckFrame = -1;
    // sElevatorsLength = 0;
    sLastElevator = nullptr;
    sLastElevatorSP = nullptr;
    // Create a heap
    // auto rootHeap = sead::HeapMgr::getRootHeaps()[0];
    // sSkylineHeap = sead::ExpHeap::create(33554432u /*32MB*/, "SkylineHeap", rootHeap, 0x1000,
    // sead::ExpHeap::cHeapDirection_Forward, false);
}

void ComputeDebugData() {
    ksys::SystemTimers* systemTimers = ksys::SystemTimers::instance();
    if (systemTimers) {
        sUnloadCheckFrame = systemTimers->mFrameCounter % 30;
        // if(systemTimers->mFrameCounter > 1800) {
        act::BaseProcMgr* baseProcMgr = ksys::act::BaseProcMgr::instance();

        if (baseProcMgr) {
            // ElevatorProcessorFunc processor {sElevators, &sElevatorsLength};
            // sead::Delegate1<ElevatorProcessorFunc, act::BaseProc*> delegate {&processor,
            // &ElevatorProcessorFunc::invoke}; sElevatorsLength = 0;
            // baseProcMgr->forEachProc("DgnObj_EntranceElevator_A_01", delegate, {});
            sLastElevator = baseProcMgr->getProc("DgnObj_EntranceElevator_A_01", {});
            sLastElevatorSP = baseProcMgr->getProc("DgnObj_EntranceElevatorSP", {});
            ;
            // //baseProcMgr->forEachProc("DgnObj_EntranceElevatorSP", processor, {});
        } else {
            // sElevatorsLength = 0;
            sLastElevator = nullptr;
            sLastElevatorSP = nullptr;
        }
        //}
    } else {
        sUnloadCheckFrame = -1;
    }
}

void RenderDebugScreen(sead::TextWriter* textWriter) {
    if (!textWriter) {
        // Dummy check so we could call this function in skyline so that the linker does not
        // optimize it out
        return;
    }
    textWriter->printf("In Game Debugger\n");
    textWriter->printf("Heap 0x%08x", sSkylineHeap);
    if (sSkylineHeap) {
        textWriter->printf("(%08x/%08x Free)", sSkylineHeap->getFreeSize(),
                           sSkylineHeap->getSize());
    }

    ksys::SystemTimers* systemTimers = ksys::SystemTimers::instance();
    textWriter->printf("ksys::SystemTimers 0x%08x\n", systemTimers);
    if (systemTimers) {
        textWriter->printf("    mFrameCounter     %d\n", systemTimers->mFrameCounter);
        textWriter->printf("    mFrameCounter2    %d\n", systemTimers->mFrameCounter2);
        textWriter->printf("    mvfrTimer     %f\n", systemTimers->mVfrTimer);
        textWriter->printf("    mFrameCounterB    %d\n", systemTimers->mFrameCounterB);
        textWriter->printf("    mvfrTimer2    %f\n", systemTimers->mVfrTimer2);
        textWriter->printf("    mFrameCounterB2   %d\n", systemTimers->mFrameCounterB2);
    }
    if (sUnloadCheckFrame != -1) {
        textWriter->printf("sUnloadCheckFrame %02d\n", sUnloadCheckFrame);
    }

    ksys::act::BaseProcMgr* baseProcMgr = ksys::act::BaseProcMgr::instance();
    textWriter->printf("ksys::act::BaseProcMgr 0x%08x\n", baseProcMgr);
    // for(s32 i=0;i<sElevatorsLength;i++){
    //     if(sElevators[i]){
    //         textWriter->printf("sElevators[%d] 0x%08x\n", i,sElevators[i] );
    //         //ID
    //         textWriter->printf("    ID 0x%08x (%02d)\n", sElevators[i]->getId(),
    //         sElevators[i]->getId() % 30); textWriter->printf("    Name %s\n",
    //         sElevators[i]->getName().cstr()); textWriter->printf("    State %d\n",
    //         sElevators[i]->getState());
    //     }
    // }
    if (sLastElevator) {
        textWriter->printf("sLastElevator 0x%08x\n", sLastElevator);
        // ID
        textWriter->printf("    ID 0x%08x (%02d)\n", sLastElevator->getId(),
                           sLastElevator->getId() % 30);
        textWriter->printf("    Name %s\n", sLastElevator->getName().cstr());
        textWriter->printf("    State %d\n", sLastElevator->getState());
    }
    if (sLastElevatorSP) {
        textWriter->printf("sLastElevatorSP 0x%08x\n", sLastElevatorSP);
        // ID
        textWriter->printf("    ID 0x%08x (%02d)\n", sLastElevatorSP->getId(),
                           sLastElevatorSP->getId() % 30);
        textWriter->printf("    Name %s\n", sLastElevatorSP->getName().cstr());
        textWriter->printf("    State %d\n", sLastElevatorSP->getState());
    }
}

}  // namespace ksys::igd
