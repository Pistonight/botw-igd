#pragma once

#include "KingSystem/ActorSystem/actAiAction.h"

namespace uking::action {

class WolfLinkAmiiboRegister : public ksys::act::ai::Action {
    SEAD_RTTI_OVERRIDE(WolfLinkAmiiboRegister, ksys::act::ai::Action)
public:
    explicit WolfLinkAmiiboRegister(const InitArg& arg);
    ~WolfLinkAmiiboRegister() override;

    bool init_(sead::Heap* heap) override;
    void enter_(ksys::act::ai::InlineParamPack* params) override;
    void leave_() override;
    void loadParams_() override;

protected:
    void calc_() override;

    // dynamic_param at offset 0x20
    int* mSpawnFlags_d{};
    // dynamic_param at offset 0x28
    sead::Vector3f* mTargetPos_d{};
};

}  // namespace uking::action
