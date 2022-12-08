#pragma once

#include <heap/seadDisposer.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace ksys::act {

class ActorConstDataAccess;

// TODO: incomplete
class ActorSystem {
    SEAD_SINGLETON_DISPOSER(ActorSystem)
    ActorSystem();

public:
    void onBaseProcMgrCalc();

    bool getPlayer(ActorConstDataAccess* accessor);

    bool getAutoPlacementActorPos(const sead::SafeString& name, sead::Vector3f* pos) const;

    sead::Heap* getEmergencyHeap() const { return mEmergencyHeap; }
    const sead::Vector3f& getPlayerPos() const { return mPlayerPos; }
    const f32 getPlayerMoveSpeed() const { return mPlayerMoveSpeed; }
    const void* mc0() const { return m_c0; }

private:
    u8 temp_0x20[0xc0 - 0x20];
    void* m_c0;
    void* m_c8;
    sead::Heap* mEmergencyHeap;
    sead::Vector3f mPlayerPos;
    f32 mPlayerMoveSpeed;
};

}  // namespace ksys::act
