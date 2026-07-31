#include "PlayerPreventionCollOperator.h"
#include "../../../../Common/Collider/XZCircleCollider.h"

PlayerPreventionCollOperator::PlayerPreventionCollOperator(
	COLLIDER_TAG COLL_TAG,
	const Vector3& playerPos,
	const float PREVENTION_RADIUS,
	MSG_SENDER_ID operatorSenderId
) :
	ActorBase(),
	COLL_TAG(COLL_TAG),
	playerPos(playerPos),
	PREVENTION_RADIUS(PREVENTION_RADIUS),
	operatorSenderId(operatorSenderId),

	isCharge(false)
{
}

void PlayerPreventionCollOperator::Load(void)
{
	ColliderCreate(
		new XZCircleCollider(
			COLL_TAG,
			PREVENTION_RADIUS,
			1000.0f,
			trans.pos,
			trans.angle
		)
	);

#pragma region 初期設定
	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(false);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);
#pragma endregion

}

void PlayerPreventionCollOperator::Update(void)
{
	trans.pos = playerPos;

}

void PlayerPreventionCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (!Net::GetIns().IsHost()) { return; }

	switch (other.GetTag())
	{
	case COLLIDER_TAG::PLAYER_ASCENTION_PREVENTION:
	{

		break;
	}
	default:
		break;
	}
}
