#include "TomatoPlayerTackleCollOperator.h"

#include "../../../../../../Common/Collider/SphereCollider.h"

TomatoPlayerTackleCollOperator::TomatoPlayerTackleCollOperator(
	COLLIDER_TAG COLL_TAG,
	const short ATTACK_RATE_PERCENT,
	const Vector3& playerPos, const Quaternion& playerAngle,
	MSG_SENDER_ID controlSenderId,
	const CharacterStats& playerStats
) :
	ActorBase(),
	COLL_TAG(COLL_TAG),
	ATTACK_RATE_PERCENT(ATTACK_RATE_PERCENT),
	playerPos(playerPos), playerAngle(playerAngle),
	controlSenderId(controlSenderId),
	playerStats(playerStats),
	isHit(false)
{
}

void TomatoPlayerTackleCollOperator::Load(void)
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
	SetJudgeFlg(false);
	isHit = false;

	// スキルのダメージ量の設定
	CreateAttackSkill(controlSenderId, ATTACK_RATE_PERCENT, &playerStats, COLL_TAG);

	playerStats;
}

void TomatoPlayerTackleCollOperator::Update(void)
{
	// 座標と角度をプレイヤーの座標に追従
	trans.pos = playerPos;
	trans.rotation = playerAngle;

}

void TomatoPlayerTackleCollOperator::SubAlphaDraw(void)
{

}

void TomatoPlayerTackleCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::Boss:
	case COLLIDER_TAG::Enemy:
	case COLLIDER_TAG::BossDistance:
		isHit = true;
		break;
	default:break;
	}
}
