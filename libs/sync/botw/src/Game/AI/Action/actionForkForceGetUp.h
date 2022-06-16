#pragma once

#include "Game/AI/Action/actionForkAlwaysForceGetUp.h"
#include "KingSystem/ActorSystem/actAiAction.h"

namespace uking::action {

class ForkForceGetUp : public ForkAlwaysForceGetUp {
    SEAD_RTTI_OVERRIDE(ForkForceGetUp, ForkAlwaysForceGetUp)
public:
    explicit ForkForceGetUp(const InitArg& arg);
    ~ForkForceGetUp() override;

    bool init_(sead::Heap* heap) override;
    void enter_(ksys::act::ai::InlineParamPack* params) override;
    void leave_() override;
    void loadParams_() override;

protected:
    void calc_() override;
};

}  // namespace uking::action
