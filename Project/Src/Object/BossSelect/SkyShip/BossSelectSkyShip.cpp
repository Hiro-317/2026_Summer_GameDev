#include "BossSelectSkyShip.h"

#include "../../../Utility/Utility.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../../Manager/Input/InputManager.h"

#include "../../../Scene/SceneManager.h"

#include "../../../Scene/BossSelect/BossConfirm/BossConfirmScene.h"

#include "../../Common/Collider/SphereCollider.h"

#include "SkyShipState/BossSelectSkyShipStartState.h"
#include "SkyShipState/BossSelectSkyShipMoveState.h"

BossSelectSkyShip::BossSelectSkyShip() :
	CharacterBase()
{
	controlSenderId = Net::HOST_SENDER_ID;
}

void BossSelectSkyShip::Load(void)
{
	SetDynamicFlg(true);
	SetGravityFlg(false);
	SetPushFlg(true);
	SetPushWeight(0);

	trans.LoadModel("SkyShip/SkyShip");

	trans.scale = 1.0f;

	trans.localRotation = Quaternion::FromRotationY(Deg2Rad(90.0f));

	trans.pos = Vector3(0, 300, -7000);

	AddCollider(new SphereCollider(COLLIDER_TAG::PLAYER, 50.0f * trans.scale.MinElementF()));
	
	AddState(
		STATE::Start,
		new BossSelectSkyShipStartState(
			[&]() { ChangeState((int)STATE::Start); },
			[&]() { return state == (int)STATE::Start; },
			Vector3(-5000, 1500, 3000),
			300,150,
			trans.pos, trans.angle,
			[&]() { ChangeState((int)STATE::Move); }
		)
	);

	AddState(
		(int)STATE::Move,
		new BossSelectSkyShipMoveState(
			[&]() { ChangeState((int)STATE::Move); },
			[&]() { return state == (int)STATE::Move; },
			30.0f, accelSum, trans.angle
		)
	);
}

void BossSelectSkyShip::SubInit(void)
{
	ChangeState(STATE::Start);
}

void BossSelectSkyShip::ReceptionUpdate(void)
{
	// ç¿ïWÅEäpìxÇÃìØä˙
	while (MsgDataPlayerTrans* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerTrans>(operatorSenderId)) {

		trans.pos = dataPtr->pos;
		trans.angle = dataPtr->angle;

		delete dataPtr;
	}
}

void BossSelectSkyShip::SendUpdate(void)
{
	if (isOwnOperator) { Net::GetIns().Send(MsgDataPlayerTrans(trans.pos, trans.angle), operatorSenderId); }
}

void BossSelectSkyShip::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (!isOwnOperator) { return; }

	if (other.GetTag() == COLLIDER_TAG::BOSS) {
		if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

			Net::GetIns().Send(MsgDataBossSelect((int)SceneManager::GetIns().GetSelectBossType()), Net::GetIns().GetSenderId());
			Net::GetIns().Send(MsgDataSystemInform(MsgDataSystemInform::INFORM_TYPE::PushSceneBossConfirm));

			SceneManager::GetIns().PushScene(std::make_unique<BossConfirmScene>());
		}
	}
}
