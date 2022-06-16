#pragma once

#include "KingSystem/ActorSystem/actAiAction.h"

namespace uking::action {

class SiteBossSwordAttackBase : public ksys::act::ai::Action {
    SEAD_RTTI_OVERRIDE(SiteBossSwordAttackBase, ksys::act::ai::Action)
public:
    explicit SiteBossSwordAttackBase(const InitArg& arg);
    ~SiteBossSwordAttackBase() override;

    bool init_(sead::Heap* heap) override;
    void enter_(ksys::act::ai::InlineParamPack* params) override;
    void leave_() override;
    void loadParams_() override;

protected:
    void calc_() override;

    // static_param at offset 0x20
    const int* mAtMinDamage_s{};
    // static_param at offset 0x28
    const int* mAttackPower_s{};
    // static_param at offset 0x30
    const int* mAddAttackPower_s{};
    // static_param at offset 0x38
    const float* mRotSpd_s{};
    // static_param at offset 0x40
    const float* mFinRotate_s{};
    // static_param at offset 0x48
    const float* mPosReduceRatio_s{};
    // static_param at offset 0x50
    const float* mBaseRotRatio_s{};
    // static_param at offset 0x58
    const float* mJustAvoidAngle_s{};
    // static_param at offset 0x60
    const float* mJustAvoidSideDist_s{};
    // static_param at offset 0x68
    const float* mJustAvoidBackDist_s{};
    // static_param at offset 0x70
    const float* mMoveSpeed_s{};
    // static_param at offset 0x78
    const float* mKeepDistance_s{};
    // static_param at offset 0x80
    const bool* mIsIgnoreCancelAttack_s{};
    // static_param at offset 0x88
    sead::SafeString mASName_s{};
    // dynamic_param at offset 0x98
    sead::Vector3f* mTargetPos_d{};
};

}  // namespace uking::action
