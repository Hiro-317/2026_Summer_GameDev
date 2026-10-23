#pragma once
#include "../GrapeBossWeaponBase.h"

class SphereCollider;

class GrapeBossSingleBomb : public GrapeBossWeaponBase
{
public:
    GrapeBossSingleBomb(int model, int front, int back);
    ~GrapeBossSingleBomb() = default;

    void Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats) override;

    const float BOMBER_COUNT = GetParameter("SingleBomb", "BomberCnt");
    const float ATTACK_DURATION = GetParameter("SingleBomb", "Duration");

    void Shot(const Vector3& pos);

protected:
    void SubUpdate(void) override;

private:
    const float MODEL_RADIUS = GetParameter("SingleBomb", "Radius");
    const float MODEL_SCALE = GetParameter("SingleBomb", "Size");
    const float RADIUS = MODEL_RADIUS * MODEL_SCALE;
    const float ATTACK_RADIUS = GetParameter("SingleBomb", "Range");

    SphereCollider* objectCollRef;
    SphereCollider* attackCollRef;

    void Blast();
};
