#include "PlayerTripleAttackCollOperator.h"

#include "../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../Common/Collider/SphereCollider.h"

#include "../../../../Common/CharacterBase/CharacterStatsDefine.h"

PlayerTripleAttackCollOperator::PlayerTripleAttackCollOperator(
	float FIND_ATTACK_TARGET_RANGE,

	COLLIDER_TAG COLL_TAG,
	float COLL_SIZE,
	const Vector3& COLL_LOCAL_POS,

	const std::array<short, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max>& ATTACK_RATE_PERCENT,

	const std::array<MsgDataPlayerCollOperator::COLLIDER_TYPE, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max>& SEND_COLLIDER_TYPE,

	const Transform& playerTrans,

	const MSG_SENDER_ID& controlSenderId,

	const CharacterStats& playerStats
):
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
	attackCollRef(),

	isFindAttackTarget(false),
	
	isHit(false),

	targetPos(nullptr)
{
}

void PlayerTripleAttackCollOperator::Load(void)
{
#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	// 探索用のコライダーを生成
	findCollRef = new SphereCollider(COLLIDER_TAG::PLAYER_COMMON, FIND_ATTACK_TARGET_RANGE);
	// 探索用のコライダーをアクターに追加
	AddCollider(findCollRef);

	// 攻撃用のコライダーを生成
	for (int i = 0; i < (int)PLAYER_TRIPLE_ATTACK_STAGE::Max; i++) {
		// 生成
		attackCollRef[i] = new SphereCollider(COLL_TAG, COLL_SIZE);
		// 攻撃用のコライダーにスキル情報を登録
		attackCollRef[i]->SetSkillStats(new SkillStats(ATTACK_RATE_PERCENT[i], &playerStats));
		// アクターに追加
		AddCollider(attackCollRef[i]);
	}

	// 当たり判定を初期化
	SetJudgeFlg(false);
}

void PlayerTripleAttackCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (ownTag == COLLIDER_TAG::PLAYER_COMMON) {
		switch (other.GetTag())
		{
		case COLLIDER_TAG::BOSS:
		case COLLIDER_TAG::ENEMY:
		case COLLIDER_TAG::BOSS_DISTANCE:

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
	else if (ownTag == COLL_TAG) {
		switch (other.GetTag())
		{
		case COLLIDER_TAG::BOSS:
		case COLLIDER_TAG::ENEMY:
		case COLLIDER_TAG::BOSS_DISTANCE:

			// 多段ヒットを防ぐ為のヒットフラグを立てる
			isHit = true;

			break;
		default:break;
		}
	}
}

void PlayerTripleAttackCollOperator::SubUpdate(void)
{
	if (!IsCollActive()) { return; }

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

void PlayerTripleAttackCollOperator::TargetSerch(void)
{
	findCollRef->SetJudgeFlg(true);
}

void PlayerTripleAttackCollOperator::CollOn(PLAYER_TRIPLE_ATTACK_STAGE stage)
{
	if (!isHit) { attackCollRef[(int)stage]->SetJudgeFlg(true); }

	if (!Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataPlayerCollOperator(true, SEND_COLLIDER_TYPE[(int)stage]));
	}
}

void PlayerTripleAttackCollOperator::CollOff(PLAYER_TRIPLE_ATTACK_STAGE stage)
{
	// 探索用の判定を消す
	findCollRef->SetJudgeFlg(false);

	if (stage == PLAYER_TRIPLE_ATTACK_STAGE::None) {
		// 指定がない場合は全ての段の判定を消す
		for (ColliderBase* coll : attackCollRef) { coll->SetJudgeFlg(false); }
	}
	// 指定がある場合はその段の判定を消す
	else { attackCollRef[(int)stage]->SetJudgeFlg(false); }

	// コライダー消去通知を送信
	if (!Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataPlayerCollOperator(false, SEND_COLLIDER_TYPE[(int)stage]));
	}
}

bool PlayerTripleAttackCollOperator::IsCollActive(void) const
{
	if (findCollRef->GetJudgeFlg()) { return true; }

	for (ColliderBase* coll : attackCollRef) {
		if (coll->GetJudgeFlg()) { return true; }
	}

	return false;
}