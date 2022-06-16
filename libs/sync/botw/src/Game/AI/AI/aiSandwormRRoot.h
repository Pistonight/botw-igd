#pragma once

#include "Game/AI/AI/aiEnemyRoot.h"
#include "KingSystem/ActorSystem/actAiAi.h"

namespace uking::ai {

class SandwormRRoot : public EnemyRoot {
    SEAD_RTTI_OVERRIDE(SandwormRRoot, EnemyRoot)
public:
    explicit SandwormRRoot(const InitArg& arg);
    ~SandwormRRoot() override;

    bool init_(sead::Heap* heap) override;
    void enter_(ksys::act::ai::InlineParamPack* params) override;
    void leave_() override;
    void loadParams_() override;

protected:
    // static_param at offset 0x1d8
    const float* mSandOffset_s{};
    // static_param at offset 0x1e0
    const float* mWeakPointDamageRate_s{};
    // static_param at offset 0x1e8
    const float* mWeakChimicalDamageRate_s{};
};

}  // namespace uking::ai
