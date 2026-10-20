#pragma once

#include "../../../../../../Common/CharacterBase/CharacterStateBase.h"

#include <functional>

#include "../../../../../../../Common/Vector3.h"

struct CharacterStats;
struct Transform;

class TomatoPlayerMoveState : public CharacterStateBase
{
public:
    
    // トマト用移動ステート：モデルを実移動距離に合わせて転がす
    TomatoPlayerMoveState(
        const CharacterStats& stats,
        float DASH_SPEED_RATE,
        short DASH_STAMINA_MAX,
        std::function<void(const Vector3&)> moveAccel,
        float& ACCEL_MAX,
        Transform& playerTrans,
        float ROLL_RADIUS);

    ~TomatoPlayerMoveState() override = default;

    void OwnStateConditionUpdate(void) override;
    void Enter(void) override;
    void Update(void) override;
    void Exit(void) override;
    void AlwaysUpdate(void) override;

    const float& GetDashStamina(void) const { return dashStamina; }
    bool IsTired(void) const { return isTired; }

private:
    const CharacterStats& stats;
    const float DASH_SPEED_RATE;
    const short DASH_STAMINA_MAX;
    const std::function<void(const Vector3&)> moveAccel;
    float& ACCEL_MAX;
    Transform& playerTrans;
    const float ROLL_RADIUS;

    bool isDash;
    float dashStamina;
    bool isTired;

    Vector3 InputVec(void) const;
    float MOVE_SPEED_MAX(bool isDash) const;
    void UpdateRolling(void);
};
