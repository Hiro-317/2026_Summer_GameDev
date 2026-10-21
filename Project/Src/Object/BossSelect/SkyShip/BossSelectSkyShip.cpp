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

	AddCollider(new SphereCollider(COLLIDER_TAG::Player, 50.0f * trans.scale.MinElementF()));
	
	AddState(
		STATE::Start,
		new BossSelectSkyShipStartState(
			Vector3(-5000, 1500, 3000),
			300,150,
			trans.pos, trans.rotation,
			[&]() { ChangeState(STATE::Move); }
		)
	);

	AddState(
		STATE::Move,
		new BossSelectSkyShipMoveState(
			std::bind(&BossSelectSkyShip::MoveAccel, this, std::placeholders::_1)
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
	while (MsgDataPlayerTrans* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerTrans>(controlSenderId)) {
		
		trans.pos = dataPtr->pos;
		trans.rotation = dataPtr->rotation;

		delete dataPtr;
	}
}

void BossSelectSkyShip::SendUpdate(void)
{
	if (IsOwnControl()) { Net::GetIns().Send(MsgDataPlayerTrans(trans.pos, trans.rotation), controlSenderId); }
}

void BossSelectSkyShip::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (!IsOwnControl()) { return; }

	if (other.GetTag() == COLLIDER_TAG::Boss) {
		if (Input::GetIns().GetInfo(KEY_TYPE::Enter).down) {

			Net::GetIns().Send(MsgDataBossSelect((int)SceneManager::GetIns().GetSelectBossType()), Net::GetIns().GetSenderId());
			Net::GetIns().Send(MsgDataSystemInform(MsgDataSystemInform::INFORM_TYPE::PushSceneBossConfirm));

			SceneManager::GetIns().PushScene(std::make_unique<BossConfirmScene>());
		}
	}
}
