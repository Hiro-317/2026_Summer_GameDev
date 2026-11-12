#include "PlayerSingleModifierCollOperator.h"

#include "../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../Common/Collider/SphereCollider.h"

#include "../../../../Common/CharacterBase/CharacterStatsDefine.h"

PlayerSingleModifierCollOperator::PlayerSingleModifierCollOperator(
	COLLIDER_TAG COLL_TAG,

	const MSG_SENDER_ID& controlSenderId,

	MsgDataPlayerCollOperator::COLLIDER_TYPE sendColliderKinds,

	const Vector3* const& targetPos,

	const short SKILL_POWER,
	const short SKILL_TIME,
	const ModifierType MODIFIER_TYPE
) :
	COLL_TAG(COLL_TAG),

	controlSenderId(controlSenderId),

	sendColliderKinds(sendColliderKinds),

	targetPos(targetPos),

	SKILL_POWER(SKILL_POWER),
	SKILL_TIME(SKILL_TIME),
	MODIFIER_TYPE(MODIFIER_TYPE),

	modifierCollRef(nullptr),

	isHit(false),

	targetTrans()
{
}

void PlayerSingleModifierCollOperator::Load(void)
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

	// コライダーを生成
	modifierCollRef = new SphereCollider(COLL_TAG, COLL_RADIUS);

	// スキル情報を登録
	if (SKILL_TIME == -1) {
		// 効果時間が未設定(-1)の場合、回復スキルとみなす
		modifierCollRef->SetSkillStats(new SkillStats(SKILL_POWER));
	}
	else {
		// 効果時間が設定されていた場合、バフ/デバフ スキルとみなす
		modifierCollRef->SetSkillStats(new SkillStats(MODIFIER_TYPE, controlSenderId, SKILL_POWER, (float)SKILL_TIME));
	}

	// アクターに追加
	AddCollider(modifierCollRef);

#pragma endregion

	// 当たり判定を切る
	SetJudgeFlg(false);
}

void PlayerSingleModifierCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::Player:
		isHit = true;
		break;
	default:break;
	}
}

void PlayerSingleModifierCollOperator::SubUpdate(void)
{
	if (!GetJudgeFlg()) { return; }

	// ヒットチェック
	if (isHit) {

		// 当たり判定を消す
		CollOff();

		// 終了
		return;
	}

	if (!targetPos) { return; }

	// 追従
	trans.pos = *targetPos;
	targetTrans.pos = *targetPos;
}

void PlayerSingleModifierCollOperator::CollOn(void)
{
	if (isHit) { return; }

	// 当たり判定開始
	SetJudgeFlg(true);

	if (!Net::GetIns().IsHost()) {
		// コライダーの種類を送信
		Net::GetIns().Send(MsgDataPlayerCollOperator(true, sendColliderKinds), controlSenderId);
	}
}

void PlayerSingleModifierCollOperator::CollOff(void)
{
	SetJudgeFlg(false);

	if (!Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataPlayerCollOperator(false, sendColliderKinds), controlSenderId);
	}
}
