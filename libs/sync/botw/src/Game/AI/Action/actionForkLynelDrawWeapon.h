#pragma once

#include "KingSystem/ActorSystem/actAiAction.h"

namespace uking::action {

class ForkLynelDrawWeapon : public ksys::act::ai::Action {
    SEAD_RTTI_OVERRIDE(ForkLynelDrawWeapon, ksys::act::ai::Action)
public:
    explicit ForkLynelDrawWeapon(const InitArg& arg);
    ~ForkLynelDrawWeapon() override;

    bool init_(sead::Heap* heap) override;
    void enter_(ksys::act::ai::InlineParamPack* params) override;
    void leave_() override;
    void loadParams_() override;

protected:
    void calc_() override;

    // static_param at offset 0x20
    const int* mWeaponIdx0_s{};
    // static_param at offset 0x28
    const int* mWeaponIdx1_s{};
    // static_param at offset 0x30
    const int* mSeqBank_s{};
    // static_param at offset 0x38
    const int* mTargetBone_s{};
    // static_param at offset 0x40
    const int* mASWeaponIdx_s{};
    // static_param at offset 0x48
    sead::SafeString mASName_s{};
};

}  // namespace uking::action
