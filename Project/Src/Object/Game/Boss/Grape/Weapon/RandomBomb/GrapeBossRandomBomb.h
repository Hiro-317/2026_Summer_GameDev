#pragma once
#include "../GrapeBossWeaponBase.h"

class SphereCollider;

class GrapeBossRandomBomb : public GrapeBossWeaponBase
{
public:
    GrapeBossRandomBomb(int model, int front, int back);
    ~GrapeBossRandomBomb() = default;

    void Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats) override;

    const float BOMBER_COUNT = GetParameter("RandomBomb", "BomberCnt");
    const float ATTACK_DURATION = GetParameter("RandomBomb", "Duration");

    void Shot(const Vector3& pos);

protected:
    void SubUpdate(void) override;

private:
    const float MODEL_RADIUS = GetParameter("RandomBomb", "Radius");
    const float MODEL_SCALE = GetParameter("RandomBomb", "Size");
    const float RADIUS = MODEL_RADIUS * MODEL_SCALE;
    const float ATTACK_RADIUS = GetParameter("RandomBomb", "Range");

    SphereCollider* objectCollRef;
    SphereCollider* attackCollRef;

    void Blast();
};
