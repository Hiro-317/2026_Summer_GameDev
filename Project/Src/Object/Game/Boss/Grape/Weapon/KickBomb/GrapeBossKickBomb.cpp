#include "GrapeBossKickBomb.h"

#include "../../../../../../Manager/TimeScale/TimeScale.h"
#include "../../../../../Common/Collider/SphereCollider.h"
#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"

GrapeBossKickBomb::GrapeBossKickBomb(int model, int front, int back) :
    GrapeBossWeaponBase(model, front, back),
    objectCollRef(nullptr),
    attackCollRef(nullptr)
{
    objectCollRef = new SphereCollider(COLLIDER_TAG::BossCommon, RADIUS);
    AddCollider(objectCollRef);

    attackCollRef = new SphereCollider(COLLIDER_TAG::BossAttack, ATTACK_RADIUS);
    AddCollider(attackCollRef);

    trans.scale = Vector3(MODEL_SCALE);
    SetGravityFlg(false);
    SetPushFlg(false);
    objectCollRef->SetJudgeFlg(false);
    attackCollRef->SetJudgeFlg(false);
    SetIsDraw(false);
    count = 0.0f;
}

void GrapeBossKickBomb::Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats)
{
    attackCollRef->SetSkillStats(new SkillStats(75, &stats));

    const float attackRadiusScale = ATTACK_RADIUS / MODEL_RADIUS;
    collBack.scale = Vector3::XZonly(attackRadiusScale, attackRadiusScale);
    collFront.scale = Vector3(0.0f);
}

void GrapeBossKickBomb::SubUpdate(void)
{
    switch (state) {
    case STATE::None:
        return;

    case STATE::Fall:
        if (isGround) {
            state = STATE::Ground;
            count = 0.0f;
        }
        break;

    case STATE::Ground:
        count += TimeScale::Get();
        if (count < BOMBER_COUNT) {
            const float progress = BOMBER_COUNT > 0.0f ? count / BOMBER_COUNT : 1.0f;
            SetViewScaleCircle((ATTACK_RADIUS / MODEL_RADIUS) * progress);
        } else {
            Blast();
            state = STATE::Blast;
            count = 0.0f;
        }
        break;

    case STATE::Blast:
        count += TimeScale::Get();
        if (count >= ATTACK_DURATION) {
            attackCollRef->SetJudgeFlg(false);
            SetIsDraw(false);
            SetViewScaleCircle(0.0f);
            count = 0.0f;
            state = STATE::None;
        }
        break;
    }
}

void GrapeBossKickBomb::Shot(const Vector3& pos)
{
    state = STATE::Fall;
    count = 0.0f;
    trans.pos = pos;
    velocity = Vector3(0.0f);
    SetIsDraw(true);
    SetViewScaleCircle(0.0f);
    objectCollRef->SetJudgeFlg(true);
    attackCollRef->SetJudgeFlg(false);
    SetGravityFlg(true);
    SetPushFlg(true);
}

void GrapeBossKickBomb::Blast()
{
    SetViewScaleCircle(0.0f);
    objectCollRef->SetJudgeFlg(false);
    attackCollRef->SetJudgeFlg(true);
    SetGravityFlg(false);
    SetPushFlg(false);
    velocity = Vector3(0.0f);
}
