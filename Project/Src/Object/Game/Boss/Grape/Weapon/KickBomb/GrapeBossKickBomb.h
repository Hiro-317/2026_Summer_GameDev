#pragma once
#include "../GrapeBossWeaponBase.h"

class SphereCollider;

class GrapeBossKickBomb : public GrapeBossWeaponBase
{
public:
    GrapeBossKickBomb(int model, int front, int back);
    ~GrapeBossKickBomb() = default;

    void Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats) override;

    const float BOMBER_COUNT = GetParameter("KickBomb", "BomberCnt");
    const float ATTACK_DURATION = GetParameter("KickBomb", "Duration");

    void Shot(const Vector3& pos);

protected:
    void SubUpdate(void) override;

private:
    const float MODEL_RADIUS = GetParameter("KickBomb", "Radius");
    const float MODEL_SCALE = GetParameter("KickBomb", "Size");
    const float RADIUS = MODEL_RADIUS * MODEL_SCALE;
    const float ATTACK_RADIUS = GetParameter("KickBomb", "Range");

    SphereCollider* objectCollRef;
    SphereCollider* attackCollRef;

    void Blast();
};
