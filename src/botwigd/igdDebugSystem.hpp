#pragma once

#include <KingSystem/ActorSystem/actBaseProcMgr.h>
#include <gfx/seadTextWriter.h>

namespace ksys::igd {

void Init();
void ComputeDebugData();
void RenderDebugScreen(sead::TextWriter* textWriter);

// class ElevatorProcessorFunc{
// public:
//     ElevatorProcessorFunc(act::BaseProc** array, s32* size){
//         mArray = array;
//         mSize = size;
//     }
//     void invoke(act::BaseProc* e){
//         if(*mSize >= 137){
//             return;
//         }
//         mArray[*mSize] = e;
//         *mSize = *mSize + 1;
//     }
// private:
//     act::BaseProc** mArray;
//     s32* mSize;
// } ;

}  // namespace ksys::igd
