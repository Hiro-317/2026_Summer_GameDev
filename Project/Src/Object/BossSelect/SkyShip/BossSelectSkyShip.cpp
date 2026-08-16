#include "BossSelectSkyShip.h"

#include "../../Common/Collider/SphereCollider.h"

#include "SkyShipState/BossSelectSkyShipMoveState.h"

BossSelectSkyShip::BossSelectSkyShip() :
	CharacterBase()
{
	isOwnOperator = true;
}

void BossSelectSkyShip::Load(void)
{
	SetDynamicFlg(true);
	SetGravityFlg(false);
	SetPushFlg(true);
	SetPushWeight(0);

	trans.Load("SkyShip/SkyShip");

	trans.scale = 1.0f;

	trans.localAngle = Vector3::Yonly(Deg2Rad(90.0f));

	trans.pos = Vector3(0, 300, -7000);

	ColliderCreate(new SphereCollider(COLLIDER_TAG::PLAYER, 50.0f * trans.scale.MinElementF()));

	AddState(
		(int)STATE::Move,
		new BossSelectSkyShipMoveState(
			[&]() { ChangeState((int)STATE::Move); },
			[&]() { return state == (int)STATE::Move; },
			30.0f, accelSum, trans.angle
		)
	);

	ChangeState((int)STATE::Move);
}
