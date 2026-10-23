#pragma once
#include "../GrapeBossWeaponBase.h"

class SphereCollider;

class GrapeBossStalkerBomb : public GrapeBossWeaponBase
{
public:
    GrapeBossStalkerBomb(int model, int front, int back);
    ~GrapeBossStalkerBomb() = default;

    void Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats) override;

    const float BOMBER_COUNT = GetParameter("StalkerBomb", "BomberCnt");
    const float ATTACK_DURATION = GetParameter("StalkerBomb", "Duration");

    void Shot(const Vector3& pos);

protected:
    void SubUpdate(void) override;

private:
    const float MODEL_RADIUS = GetParameter("StalkerBomb", "Radius");
    const float MODEL_SCALE = GetParameter("StalkerBomb", "Size");
    const float RADIUS = MODEL_RADIUS * MODEL_SCALE;
    const float ATTACK_RADIUS = GetParameter("StalkerBomb", "Range");

    SphereCollider* objectCollRef;
    SphereCollider* attackCollRef;

    void Blast();
};
