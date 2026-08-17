#include "BossSelectSkyShip.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../../Manager/Input/KeyManager.h"

#include "../../../Scene/SceneManager/SceneManager.h"

#include "../../../Scene/BossSelect/BossConfirm/BossConfirmScene.h"

#include "../../Common/Collider/SphereCollider.h"

#include "SkyShipState/BossSelectSkyShipMoveState.h"

BossSelectSkyShip::BossSelectSkyShip() :
	CharacterBase(),

	pushScene(false)
{
	operatorSenderId = Net::HOST_SENDER_ID;
	isOwnOperator = operatorSenderId == Net::GetIns().GetSenderId();
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

void BossSelectSkyShip::ReceptionUpdate(void)
{
	// 座標・角度の同期
	while (MsgDataPlayerTrans* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerTrans>(operatorSenderId)) {
		// 自分のキャラ（操作対象）の場合
		if (isOwnOperator) {
			// ホストから送られた座標と今の自分の座標の距離を測る
			float diff = (trans.pos - dataPtr->pos).Length();

			// 誤差が小さいなら無視
			if (diff > 0.5f) {
				// 誤差が大きい場合、少しずつホストから送られた座標に寄せる（補間）
				trans.pos = trans.pos * 0.9f + dataPtr->pos * 0.1f;
			}

			// ホストから送られた角度と今の自分の角度のずれを測る
			diff = (trans.angle - dataPtr->angle).Length();

			// 誤差が小さいなら無視
			if (diff > 0.5f) {
				// 誤差が大きい場合、少しずつホストから送られた角度を寄せる（補間）
				trans.angle = trans.angle * 0.9f + dataPtr->angle * 0.1f;
			}

		}
		// 他人のキャラなら、そのまま同期
		else {
			trans.pos = dataPtr->pos;
			trans.angle = dataPtr->angle;
		}


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
			SceneManager::GetIns().PushScene(std::make_unique< BossConfirmScene>());
			pushScene = true;
		}
	}
}
