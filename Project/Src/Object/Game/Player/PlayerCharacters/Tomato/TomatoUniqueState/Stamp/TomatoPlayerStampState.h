#pragma once

#include "../../../../../../Common/CharacterBase/CharacterStateBase.h"

class PlayerSimpleAttackCollOperator;
struct Transform;
struct Vector3;

class TomatoPlayerStampState : public CharacterStateBase
{
public:
    TomatoPlayerStampState(
        Transform& playerTrans,
        PlayerSimpleAttackCollOperator& collOperator,
        const std::function<void(const Vector3&)>& moveAccel,
        float& accelMax,
        Vector3& velocity,
        int coolTime,
        float jumpPower,
        float fallSpeed,
        const std::function<void(bool)>& setIsGravity,
        const std::function<void(void)>& defaultChangeState);

    ~TomatoPlayerStampState() override = default;
    void OwnStateConditionUpdate(void) override;
    void Enter(void) override;
    void Update(void) override;
    void Exit(void) override;
    void AlwaysUpdate(void) override;

    // 衝突処理中の接地通知を受け、着地したそのフレームで攻撃を有効にする。
    void OnGrounded(void);
    // 空中でも判定の位置を更新して、着地前に空間分割へ正しい位置を登録する。
    void UpdateColliderPosition(void);
    const float& GetCoolTimeCounter(void) const { return coolTimeCounter; }

private:
    enum class STEP { Rising, Falling, Landed };
    const int COOL_TIME;
    const float JUMP_POWER;
    const float FALL_SPEED;
    static constexpr float GRAVITY = 0.6f;
    static constexpr float JUMP_MOVE_SPEED = 4.0f;
    static constexpr float LAND_ATTACK_TIME = 2.0f;

    Transform& playerTrans;
    PlayerSimpleAttackCollOperator& collOperator;
    const std::function<void(const Vector3&)> moveAccel;
    float& accelMax;
    Vector3& velocity;
    const std::function<void(bool)> setIsGravity;
    const std::function<void(void)> defaultChangeState;

    // 上限値はステート生成時ではなく、攻撃開始時の設定を保存する。
    float defaultAccelMax;
    STEP step;
    float landingCounter;
    float coolTimeCounter;

    void JumpMove(void);
};