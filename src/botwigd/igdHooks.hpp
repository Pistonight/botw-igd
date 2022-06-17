#pragma once

namespace ksys {
namespace act {
struct Actor;
}
namespace igd {
struct DebugData;
}
}  // namespace ksys

namespace ksys::igd::hook {

void Init();
extern void* act__Actor__shouldUnloadBecauseOfDistance();
bool ShouldUnloadBcDistance(act::Actor* actorThis, unsigned int* out_reason);

}  // namespace ksys::igd::hook
