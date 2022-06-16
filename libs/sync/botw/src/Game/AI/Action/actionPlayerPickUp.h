#pragma once

#include "Game/AI/Action/actionPlayerAction.h"
#include "KingSystem/ActorSystem/actAiAction.h"

namespace uking::action {

class PlayerPickUp : public PlayerAction {
    SEAD_RTTI_OVERRIDE(PlayerPickUp, PlayerAction)
public:
    explicit PlayerPickUp(const InitArg& arg);
    ~PlayerPickUp() override;

    bool init_(sead::Heap* heap) override;
    void enter_(ksys::act::ai::InlineParamPack* params) override;
    void leave_() override;

protected:
    void calc_() override;
};

}  // namespace uking::action
