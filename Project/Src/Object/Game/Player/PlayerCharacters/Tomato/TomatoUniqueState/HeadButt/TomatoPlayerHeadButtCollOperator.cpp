#include "TomatoPlayerHeadButtCollOperator.h"

#include "../../../../../../Common/Collider/SphereCollider.h"

TomatoPlayerHeadButtCollOperator::TomatoPlayerHeadButtCollOperator(
	COLLIDER_TAG COLL_TAG,
	const short ATTACK_RATE_PERCENT,
	const Vector3& playerPos, const Vector3& playerAngle,
	MSG_SENDER_ID operatorSenderId,
	const CharacterStats& playerStats
) :
	ActorBase(),
	COLL_TAG(COLL_TAG),
	ATTACK_RATE_PERCENT(ATTACK_RATE_PERCENT),
	playerPos(playerPos), playerAngle(playerAngle),
	operatorSenderId(operatorSenderId),
	playerStats(playerStats),
	isHit(false)
{
}

void TomatoPlayerHeadButtCollOperator::Load(void)
{
#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	// コライダー生成
	AddCollider(new SphereCollider(COLL_TAG, 100.0f));

	// 初期化処理
	SetJudge(false);
	isHit = false;

	// スキルのダメージ量の設定
	CreateAttackSkill(operatorSenderId, ATTACK_RATE_PERCENT, &playerStats, COLL_TAG);
}

void TomatoPlayerHeadButtCollOperator::Update(void)
{
	// 座標と角度をプレイヤーの座標に追従
	trans.pos = playerPos;
	trans.angle = playerAngle;
}

void TomatoPlayerHeadButtCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::BOSS:
	case COLLIDER_TAG::ENEMY:
	case COLLIDER_TAG::BOSS_DISTANCE:
		isHit = true;
		break;
	default:break;
	}
}
