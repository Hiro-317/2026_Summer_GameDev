#include "PlayerTripleAttackCollOperator.h"

#include "../../../../Common/Collider/SphereCollider.h"

PlayerTripleAttackCollOperator::PlayerTripleAttackCollOperator(
	float FIND_ATTACK_TARGET_RANGE,
	const std::array<TAG, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_TAG,
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_SIZE,
	const Vector3& COLL_LOCAL_POS,

	const Vector3& playerPos, const Vector3& playerAngle
) :
	FIND_ATTACK_TARGET_RANGE(FIND_ATTACK_TARGET_RANGE),

	COLL_TAG(COLL_TAG), COLL_SIZE(COLL_SIZE),
	COLL_LOCAL_POS(COLL_LOCAL_POS),

	playerPos(playerPos), playerAngle(playerAngle),

	isFindAttackTarget(false),
	targetPos(nullptr)
{
}

void PlayerTripleAttackCollOperator::Load(void)
{
#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(false);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	// 当たり判定情報を生成する
	for (int i = 0; i < (int)PLAYER_TRIPLE_ATTACK_STAGE::MAX; i++) {
		ColliderCreate(new SphereCollider(COLL_TAG[i], COLL_SIZE[i], COLL_SIZE[i], COLL_LOCAL_POS));
	}
	ColliderCreate(new SphereCollider(TAG::PLAYER_TRIPLE_ATTACK_TARGET_SERCH, FIND_ATTACK_TARGET_RANGE, FIND_ATTACK_TARGET_RANGE));
	SetJudge(false);
}

void PlayerTripleAttackCollOperator::OnCollision(const ColliderBase& other)
{
	switch (other.GetTag())
	{
	case ColliderBase::TAG::BOSS:
	case ColliderBase::TAG::ENEMY:
	case ColliderBase::TAG::SPHERE_DEBUG_OBJECT:
		isFindAttackTarget = true;
		targetPos = &other.GetTransform().pos;
		ColliderSerch(TAG::PLAYER_TRIPLE_ATTACK_TARGET_SERCH).back()->SetJudgeFlg(false);
		break;
	default:break;
	}
}

void PlayerTripleAttackCollOperator::SubUpdate(void)
{
	trans.pos = playerPos;
	trans.angle = playerAngle;
	ColliderSerch(TAG::PLAYER_TRIPLE_ATTACK_TARGET_SERCH).back()->SetJudgeFlg(false);
}