#include "PlayerSimpleAttackCollOperator.h"

#include "../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../Common/Collider/SphereCollider.h"

#include "../../../../Common/CharacterBase/CharacterStatsDefine.h"

PlayerSimpleAttackCollOperator::PlayerSimpleAttackCollOperator(

	float FIND_ATTACK_TARGET_RANGE,

	COLLIDER_TAG COLL_TAG,
	float COLL_SIZE,
	const Vector3& COLL_LOCAL_POS,

	short ATTACK_RATE_PERCENT,

	MsgDataPlayerCollOperator::COLLIDER_TYPE SEND_COLLIDER_TYPE,

	const Transform& playerTrans,

	const MSG_SENDER_ID& controlSenderId,

	const CharacterStats& playerStats
) :
	ActorBase(),

	FIND_ATTACK_TARGET_RANGE(FIND_ATTACK_TARGET_RANGE),

	COLL_TAG(COLL_TAG),
	COLL_SIZE(COLL_SIZE),
	COLL_LOCAL_POS(COLL_LOCAL_POS),

	ATTACK_RATE_PERCENT(ATTACK_RATE_PERCENT),

	SEND_COLLIDER_TYPE(SEND_COLLIDER_TYPE),

	playerTrans(playerTrans),

	controlSenderId(controlSenderId),

	playerStats(playerStats),

	findCollRef(nullptr),
	attackCollRef(nullptr),

	isFindAttackTarget(false),
	targetPos(nullptr),
	isHit(false)
{
}

void PlayerSimpleAttackCollOperator::Load(void)
{
#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

#pragma region 当たり判定情報を生成する

	// 探索用のコライダーを生成
	findCollRef = new SphereCollider(COLLIDER_TAG::PlayerCommon, FIND_ATTACK_TARGET_RANGE);
	// 探索用のコライダーをアクターに追加
	AddCollider(findCollRef);

	// 攻撃用のコライダーを生成
	attackCollRef = new SphereCollider(COLL_TAG, COLL_SIZE, COLL_LOCAL_POS);
	// 攻撃用のコライダーにスキル情報を登録
	attackCollRef->SetSkillStats(new SkillStats(ATTACK_RATE_PERCENT, &playerStats));
	// 攻撃用のコライダーをアクターに追加
	AddCollider(attackCollRef);

#pragma endregion

	// 当たり判定を切る
	SetJudgeFlg(false);
}

void PlayerSimpleAttackCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (ownTag == COLLIDER_TAG::PlayerCommon) {
		switch (other.GetTag())
		{
		case COLLIDER_TAG::Boss:
		case COLLIDER_TAG::Enemy:
		case COLLIDER_TAG::BossDistance:

			// 探索結果を保存
			isFindAttackTarget = true;

			// ターゲット座標の参照を保持
			targetPos = &other.GetActor()->GetTrans().pos;

			// 探索用のコライダーの判定を消去
			findCollRef->SetJudgeFlg(false);

			break;
		default:break;
		}
	}
	else if(ownTag == COLL_TAG) {
		switch (other.GetTag())
		{
		case COLLIDER_TAG::Boss:
		case COLLIDER_TAG::Enemy:
		case COLLIDER_TAG::BossDistance:

			// 多段ヒットを防ぐ為のヒットフラグを立てる
			isHit = true;

			break;
		default:break;
		}
	}
}

void PlayerSimpleAttackCollOperator::SubUpdate(void)
{
	if (!findCollRef->GetJudgeFlg() && !attackCollRef->GetJudgeFlg()) { return; }

	// ヒットチェック
	if (isHit) {

		// 当たり判定を消す
		CollOff();

		// 終了
		return;
	}

	// 追従
	trans.pos = playerTrans.pos;
	trans.rotation = playerTrans.rotation;
}

void PlayerSimpleAttackCollOperator::TargetSerch(void)
{
	findCollRef->SetJudgeFlg(true);
}

void PlayerSimpleAttackCollOperator::CollOn(bool sendNetwork)
{
	if (!isHit) { attackCollRef->SetJudgeFlg(true); }

	if (sendNetwork && !Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataPlayerCollOperator(true, SEND_COLLIDER_TYPE));
	}
}

// 攻撃の判定を消す
void PlayerSimpleAttackCollOperator::CollOff(bool sendNetwork) 
{
	findCollRef->SetJudgeFlg(false);
	attackCollRef->SetJudgeFlg(false);

	if (sendNetwork && !Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataPlayerCollOperator(false, SEND_COLLIDER_TYPE));
	}
}
