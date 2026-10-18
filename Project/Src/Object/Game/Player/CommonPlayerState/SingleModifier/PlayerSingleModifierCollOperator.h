#pragma once

#include "../../../../Common/ActorBase/ActorBase.h"
#include "../../../../../Manager/Effect/EffectManager.h"
#include "../../../../../Manager/Net/NetWorkManager.h"

class SphereCollider;

class PlayerSingleModifierCollOperator : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="COLL_TAG">コライダー判別用タグ</param>
	/// <param name="controlSenderId">送信者ID</param>
	/// <param name="sendColliderKinds">コライダーの種類</param>
	/// <param name="targetPos">バフ・回復対象の座標</param>
	/// <param name="SKILL_POWER">バフ・回復量</param>
	/// <param name="SKILL_TIME">効果時間（-1ならヒールとして判断する</param>
	/// <param name="MODIFIER_TYPE">バフタイプ</param>
	PlayerSingleModifierCollOperator(
		COLLIDER_TAG COLL_TAG,

		const MSG_SENDER_ID& controlSenderId,

		MsgDataPlayerCollOperator::COLLIDER_TYPE sendColliderKinds,

		const Vector3* const& targetPos,

		const short SKILL_POWER,
		const short SKILL_TIME = -1,
		const ModifierType MODIFIER_TYPE = ModifierType::None
	);

	// デストラクタ
	~PlayerSingleModifierCollOperator()override = default;

	// ロード
	void Load(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 攻撃の判定を発生させる
	void CollOn(void);

	// 攻撃の判定を消す
	void CollOff(void);

	// 攻撃のヒット管理のフラグをリセットする
	void ResetIsHit(void) { isHit = false; }

private:

#pragma region 定数
	
	// コライダー判別用タグ
	const COLLIDER_TAG COLL_TAG;

	// 当たり判定の半径
	const float COLL_RADIUS = 1.0f;

	// バフタイプ
	const ModifierType MODIFIER_TYPE;

	// 効果量
	const short SKILL_POWER;

	// 効果時間
	const short SKILL_TIME;

#pragma endregion

#pragma region 受け取る参照変数

	// 送信ID
	const MSG_SENDER_ID& controlSenderId;

	// 送信するコライダー種類
	const MsgDataPlayerCollOperator::COLLIDER_TYPE sendColliderKinds;

	// バフ・回復対象の座標
	const Vector3* const& targetPos;

#pragma endregion

	// 生成するコライダーの参照
	SphereCollider* modifierCollRef;

	// 攻撃のヒット管理のフラグ
	bool isHit;

	// バフ・回復対象のtrans（エフェクトマネージャーの引数用変数）
	Transform targetTrans;

	// 更新処理
	void SubUpdate(void)override;
};