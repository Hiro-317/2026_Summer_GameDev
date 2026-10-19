#pragma once

#include "../../../../Common/ActorBase/ActorBase.h"

#include "../../../../Common/Collider/ColliderTagDefine.h"

#include "../../../../../Manager/Net/NetWorkDefine.h"

class SphereCollider;

struct CharacterStats;

class PlayerSimpleAttackCollOperator : public ActorBase
{
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="FIND_ATTACK_TARGET_RANGE">攻撃対象を見つける距離</param>
	/// <param name="COLL_TAG">攻撃のタグ</param>
	/// <param name="COLL_SIZE">攻撃判定のサイズ（半径）</param>
	/// <param name="COLL_LOCAL_POS">攻撃判定のローカル座標</param>
	/// <param name="ATTACK_RATE_PERCENT">攻撃倍率</param>
	/// <param name="SEND_COLLIDER_TYPE">攻撃を送信する際のタイプ</param>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="playerRotation">プレイヤーの向き</param>
	/// <param name="controlSenderId">操作権</param>
	/// <param name="playerStats">ステータス</param>
	PlayerSimpleAttackCollOperator(

		float FIND_ATTACK_TARGET_RANGE,

		COLLIDER_TAG COLL_TAG, 
		float COLL_SIZE,
		const Vector3& COLL_LOCAL_POS,

		short ATTACK_RATE_PERCENT,

		MsgDataPlayerCollOperator::COLLIDER_TYPE SEND_COLLIDER_TYPE,

		const Transform& playerTrans,

		const MSG_SENDER_ID& controlSenderId,

		const CharacterStats& playerStats
	);
	~PlayerSimpleAttackCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 攻撃対象を探索
	void TargetSerch(void);

	// 攻撃対象が見つかったかどうかを取得
	bool GetIsFindAttackTarget(void)const { return isFindAttackTarget; }
	// 攻撃対象の座標を取得
	const Vector3& GetTargetPos(void)const { return *targetPos; }

	// 攻撃の判定を発生させる
	void CollOn(void);

	// 攻撃の判定を消す
	void CollOff(void);

	// 攻撃対象をリセットする
	void ResetTarget(void) { isFindAttackTarget = false; targetPos = nullptr; }

	// 攻撃のヒット管理のフラグをリセットする
	void ResetIsHit(void) { isHit = false; }

private:

#pragma region 定数

	// 攻撃対象を見つける距離
	const float FIND_ATTACK_TARGET_RANGE;

	// 攻撃のタグ
	const COLLIDER_TAG COLL_TAG;

	// 攻撃判定のサイズ（半径）
	const float COLL_SIZE;

	// 攻撃判定のローカル座標
	const Vector3 COLL_LOCAL_POS;

	// 攻撃倍率
	const short ATTACK_RATE_PERCENT;

	// 攻撃を送信する際のタイプ
	MsgDataPlayerCollOperator::COLLIDER_TYPE SEND_COLLIDER_TYPE;

#pragma endregion

#pragma region 受け取る参照変数

	// プレイヤー座標の参照
	const Transform& playerTrans;

	// 操作権
	const MSG_SENDER_ID& controlSenderId;

	// ステータス
	const CharacterStats& playerStats;

#pragma endregion

	// 探索用のコライダーの参照用
	SphereCollider* findCollRef; 
	// 攻撃用のコライダーの参照用
	SphereCollider* attackCollRef;

	// 攻撃対象が見つかったかどうか
	bool isFindAttackTarget;

	// 攻撃対象の座標の参照用
	const Vector3* targetPos;

	// 攻撃のヒット管理のフラグ
	bool isHit;

	// 更新処理
	void SubUpdate(void)override;
};