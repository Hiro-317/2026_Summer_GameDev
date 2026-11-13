#include "GrapeBossWeaponBase.h"

#include "../../../../Common/Collider/ColliderBase.h"

GrapeBossWeaponBase::GrapeBossWeaponBase(int model, int front, int back)
	: ActorBase("Data/Parameter/Character/Boss/Grape/Weapons/"), collBack(), collFront(),
      count(0.0f), end(false), now(false), insRef(nullptr)
{
	trans.model = model;
	collFront.model = front;
	collBack.model = back;

	insRef = new ExplosionActor(trans.pos);
	AddChildActor(insRef);
}

void GrapeBossWeaponBase::SetColliderFlg(bool flg)
{
	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttackArea).back()->SetJudgeFlg(flg);
	SetIsDraw(flg);
}

void GrapeBossWeaponBase::SubDraw(void)
{
	SetUseLighting(false);
	collBack.Draw();
	collFront.Draw();
	SetUseLighting(true);
}

void GrapeBossWeaponBase::BeginExplosionAttack(void)
{
    auto* attack = ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back();
    // 判定がOFF→ONになる瞬間だけ再生し、毎フレーム経過時間をリセットしない。
    if (!attack->GetJudgeFlg() && insRef != nullptr) {
        insRef->SetPos(trans.pos);
        insRef->Play();
    }
    attack->SetJudgeFlg(true);
}

void GrapeBossWeaponBase::UpdateInactiveEffects(void)
{
    // 弾が消えた後も、爆発の寿命が終わるまでは更新を継続する。
    if (insRef != nullptr) { insRef->Update(); }
}
