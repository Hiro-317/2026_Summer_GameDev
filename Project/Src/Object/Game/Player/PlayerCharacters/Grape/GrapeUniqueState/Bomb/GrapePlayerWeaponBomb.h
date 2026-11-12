#pragma once

#include "../../../../../../Common/ActorBase/ActorBase.h"

struct CharacterStats;
class SphereCollider;
class PurpleExplosionActor;

class GrapePlayerWeaponBomb : public ActorBase
{
public:
    GrapePlayerWeaponBomb(short ATTACK_RATE_PERCENT, const CharacterStats& playerStats,
        float ATTACK_COUNT_TIME, const Transform& playerTrans, const MSG_SENDER_ID& controlSenderId);
    ~GrapePlayerWeaponBomb() override = default;

    void Load(void) override;
    void Init(void) override;
    void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;
    void SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other) override;
    void Put(const Vector3& pos, bool send = false);
    void Blast(void);
    void Blast(const Vector3& pos, bool send = false);
    void ReceptionUpdate(void) override;

private:
    enum class STATE { None, Put, Blast };
    const short ATTACK_RATE_PERCENT;
    const float ATTACK_COUNT_TIME;
    static constexpr float PUT_RADIUS = 100.0f;
    static constexpr float BLAST_RADIUS = 500.0f;
    static constexpr float BLAST_ATTACK_TIME = 2.0f;
    static constexpr float BLAST_EFFECT_TIME = 48.0f;

    const CharacterStats& playerStats;
    const MSG_SENDER_ID& controlSenderId;
    float lifeCounter;
    STATE state;
    SphereCollider* triggerCollider;
    SphereCollider* attackCollider;
    SphereCollider* terrainCollider;
    // 所有権は子Actorのリストへ渡す。ここでは操作用の参照だけを保持する。
    PurpleExplosionActor* explosion;

    void SubUpdate(void) override;
};