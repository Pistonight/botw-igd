#pragma once

#include "KingSystem/ActorSystem/actAiQuery.h"

namespace uking::query {

class CheckPlayerHellType : public ksys::act::ai::Query {
    SEAD_RTTI_OVERRIDE(CheckPlayerHellType, Query)
public:
    explicit CheckPlayerHellType(const InitArg& arg);
    ~CheckPlayerHellType() override;
    int doQuery() override;

    void loadParams() override;
    void loadParams(const evfl::QueryArg& arg) override;

protected:
    int* mPlayerHellType{};
};

}  // namespace uking::query
