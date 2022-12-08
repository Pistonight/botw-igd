#pragma once

#include <KingSystem/ActorSystem/actBaseProcMgr.h>
#include <gfx/seadTextWriter.h>

namespace ksys::igd {

void Init();
void ComputeDebugData();
void RenderDebugScreen(sead::TextWriter* textWriter);

}  // namespace ksys::igd
