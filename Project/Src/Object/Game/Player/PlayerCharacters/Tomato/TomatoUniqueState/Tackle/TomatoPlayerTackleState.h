#pragma once

#include "../../../../../../Common/CharacterBase/CharacterStateBase.h"
#include "../../../../../../../Common/Vector3.h"

class PlayerSimpleAttackCollOperator;
class RushAuraActor;
struct Transform;

class TomatoPlayerTackleState : public CharacterStateBase
{
public:
    TomatoPlayerTackleState(
        PlayerSimpleAttackCollOperator& collOperator,
        Transform& playerTrans,
        RushAuraActor& aura,
        Vector3& velocity,
        float moveSpeed,
        float rotationPower,
        int coolTime,
        std::function<void(void)> chargeAttackBuff,
        std::function<void(void)> chargeAttackBuffDelete,
        std::function<void(void)> defaultChangeState);

    ~TomatoPlayerTackleState() override = default;
    void OwnStateConditionUpdate(void) override;
    void Enter(void) override;
    void Update(void) override;
    void Exit(void) override;
    void AlwaysUpdate(void) override;

    const float& GetCoolTimeCounter(void) const { return coolTimeCounter; }

private:
    enum class STEP { Charge, Tackle };

    const float MOVE_SPEED;
    const float ROTATION_POW;
    const int COOL_TIME;
    static constexpr float COUNT_MAX = 20.0f;
    static constexpr float CHARGE_TIME = 60.0f;

    Transform& playerTrans;
    Vector3& velocity;
    PlayerSimpleAttackCollOperator& collOperator;
    RushAuraActor& aura;

    // ‰ñ“]p¨‚©‚ç“Ëi•ûŒü‚ğÄŒvZ‚¹‚¸A…•½‚ÌŒü‚«‚Æ‘O“]Šp‚ğ•ÊX‚É‚ÂB
    Vector3 moveDir;
    float heading;
    float spinAngle;
    float timeCounter;
    float chargeCounter;
    float coolTimeCounter;
    STEP step;

    const std::function<void(void)> chargeAttackBuff;
    const std::function<void(void)> chargeAttackBuffDelete;
    const std::function<void(void)> defaultChangeState;

    void StartTackle(void);
    void Tackle(void);
    void UpdateRotation(void);
    Vector3 InputVec(void) const;
};