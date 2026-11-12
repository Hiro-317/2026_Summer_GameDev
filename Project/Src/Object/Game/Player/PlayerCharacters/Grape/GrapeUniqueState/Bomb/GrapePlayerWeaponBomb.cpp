#include "GrapePlayerWeaponBomb.h"

#include "../../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../../Manager/TimeScale/TimeScale.h"
#include "../../../../../../Common/Collider/SphereCollider.h"
#include "../../../../../../Common/CharacterBase/CharacterStatsDefine.h"
#include "../../../../../../Common/Effect/PurpleExplosionActor.h"

GrapePlayerWeaponBomb::GrapePlayerWeaponBomb(
    short ATTACK_RATE_PERCENT, const CharacterStats& playerStats,
    float ATTACK_COUNT_TIME, const Transform& playerTrans, const MSG_SENDER_ID& controlSenderId
) :
    ActorBase(),
    ATTACK_RATE_PERCENT(ATTACK_RATE_PERCENT),
    ATTACK_COUNT_TIME(ATTACK_COUNT_TIME),
    playerStats(playerStats),
    controlSenderId(controlSenderId),
    lifeCounter(0.0f),
    state(STATE::None),
    triggerCollider(nullptr),
    attackCollider(nullptr),
    terrainCollider(nullptr),
    explosion(nullptr)
{
    ActorBase::controlSenderId = controlSenderId;
}

void GrapePlayerWeaponBomb::Load(void)
{
    SetDynamicFlg(true);
    SetGravityFlg(false);
    SetPushFlg(false);
    trans.LoadModel("Character/Grape/Bomb");
    trans.scale = 0.5f;

    // 設置中は接触を検知するだけで、ダメージは発生させない。
    triggerCollider = new SphereCollider(COLLIDER_TAG::PlayerCommon, PUT_RADIUS);
    triggerCollider->SetPushFlg(false);
    AddCollider(triggerCollider);

    // 爆発範囲は最初から登録し、接触による起爆でも空間分割に反映させる。
    attackCollider = new SphereCollider(COLLIDER_TAG::PlayerAttack, BLAST_RADIUS);
    attackCollider->SetSkillStats(new SkillStats(ATTACK_RATE_PERCENT, &playerStats));
    attackCollider->SetPushFlg(false);
    AddCollider(attackCollider);

    terrainCollider = new SphereCollider(COLLIDER_TAG::BossAttackArea, PUT_RADIUS * 0.5f);
    terrainCollider->SetPushFlg(true);
    AddCollider(terrainCollider);

    explosion = new PurpleExplosionActor(Vector3(), BLAST_RADIUS, BLAST_EFFECT_TIME / 60.0f);
    AddChildActor(explosion);
    SetJudgeFlg(false);
    SetIsDraw(false);
}

void GrapePlayerWeaponBomb::Init(void)
{
    ActorBase::Init();
    // 子Actorの初期化で再生状態になるため、その後に待機させる。
    explosion->Stop();
}

void GrapePlayerWeaponBomb::SubUpdate(void)
{
    if (Net::GetIns().GetState() != Net::NetState::None) { ReceptionUpdate(); }
    if (state == STATE::Put) {
        lifeCounter -= TimeScale::Get();
        if (lifeCounter <= 0.0f) { Blast(trans.pos, Net::GetIns().IsHost()); }
    }
    else if (state == STATE::Blast) {
        lifeCounter += TimeScale::Get();
        if (lifeCounter >= BLAST_ATTACK_TIME) { attackCollider->SetJudgeFlg(false); }
        if (lifeCounter >= BLAST_EFFECT_TIME) {
            state = STATE::None;
            SetJudgeFlg(false);
            explosion->Stop();
        }
    }
}

void GrapePlayerWeaponBomb::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
    if (!Net::GetIns().IsHost()) { return; }
    const auto tag = other.GetTag();
    const bool isBoss = tag == COLLIDER_TAG::Boss || tag == COLLIDER_TAG::BossDistance || tag == COLLIDER_TAG::Enemy;
    if (state == STATE::Put && ownTag == COLLIDER_TAG::PlayerCommon && isBoss) {
        Blast();
    }
    else if (state == STATE::Blast && ownTag == COLLIDER_TAG::PlayerAttack &&
        (tag == COLLIDER_TAG::BossDistance || tag == COLLIDER_TAG::Enemy)) {
        // 同じ爆発で複数フレーム連続してダメージを与えない。
        attackCollider->SetJudgeFlg(false);
    }
}

void GrapePlayerWeaponBomb::SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other)
{
    if (state != STATE::Put || other.GetTag() != COLLIDER_TAG::Stage || velocity.y > 0.0f) { return; }
    // 設置時は真下に落として接地させ、その後はその位置に固定する。
    velocity = Vector3();
    SetGravityFlg(false);
    terrainCollider->SetJudgeFlg(false);
}

void GrapePlayerWeaponBomb::Put(const Vector3& pos, bool send)
{
    trans.pos = pos;
    trans.prevPos = pos;
    velocity = Vector3();
    lifeCounter = ATTACK_COUNT_TIME;
    state = STATE::Put;
    explosion->Stop();
    SetIsDraw(true);
    SetGravityFlg(true);
    SetPushFlg(true);
    triggerCollider->SetJudgeFlg(true);
    terrainCollider->SetJudgeFlg(true);
    attackCollider->SetJudgeFlg(false);
    if (send) { Net::GetIns().Send(MsgDataGrapePlayerBomb(pos), controlSenderId); }
}

void GrapePlayerWeaponBomb::Blast(void)
{
    Blast(trans.pos, true);
}

void GrapePlayerWeaponBomb::Blast(const Vector3& pos, bool send)
{
    if (state != STATE::Put) { return; }
    trans.pos = pos;
    velocity = Vector3();
    lifeCounter = 0.0f;
    state = STATE::Blast;
    SetGravityFlg(false);
    SetPushFlg(false);
    SetIsDraw(false);
    triggerCollider->SetJudgeFlg(false);
    terrainCollider->SetJudgeFlg(false);
    attackCollider->SetJudgeFlg(true);
    explosion->SetPos(pos);
    explosion->Restart();
    if (send) { Net::GetIns().Send(MsgDataGrapePlayerBombBlast(pos), controlSenderId); }
}

void GrapePlayerWeaponBomb::ReceptionUpdate(void)
{
    while (auto* data = Net::GetIns().GetMsgData<MsgDataGrapePlayerBomb>(controlSenderId, true)) {
        Put(data->pos);
        delete data;
    }
    while (auto* data = Net::GetIns().GetMsgData<MsgDataGrapePlayerBombBlast>(controlSenderId)) {
        Blast(data->pos);
        delete data;
    }
}