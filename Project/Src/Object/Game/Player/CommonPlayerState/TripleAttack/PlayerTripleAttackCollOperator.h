#pragma once

#include "../../../../Common/ActorBase/ActorBase.h"

#include <array>

#include "../../../../../Manager/Net/NetWorkDefine.h"

#include "../../../../Common/Collider/ColliderTagDefine.h"

#include "PlayerTripleAttackStDefine.h"

struct CharacterStats;

class SphereCollider;

class PlayerTripleAttackCollOperator : public ActorBase
{
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="FIND_ATTACK_TARGET_RANGE">攻撃対象の探索範囲</param>
	/// <param name="COLL_TAG">攻撃用コライダーのタグ</param>
	/// <param name="COLL_SIZE">攻撃判定のサイズ（球状の半径）</param>
	/// <param name="COLL_LOCAL_POS">攻撃判定の座標（プレイヤー座標からのローカル座標）</param>
	/// <param name="ATTACK_RATE_PERCENT">攻撃倍率配列</param>
	/// <param name="playerTrans">プレイヤーのモデル制御情報の参照</param>
	/// <param name="controlSenderId">操作権の参照</param>
	/// <param name="playerStats">プレイヤーのステータス</param>
	PlayerTripleAttackCollOperator(
		float FIND_ATTACK_TARGET_RANGE,

		COLLIDER_TAG COLL_TAG,
		float COLL_SIZE,
		const Vector3& COLL_LOCAL_POS,

		const std::array<short, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max>& ATTACK_RATE_PERCENT,

		const std::array<MsgDataPlayerCollOperator::COLLIDER_TYPE, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max>& SEND_COLLIDER_TYPE,

		const Transform& playerTrans,

		const MSG_SENDER_ID& controlSenderId,

		const CharacterStats& playerStats
	);

	~PlayerTripleAttackCollOperator()override {
		findCollRef = nullptr;
		for (SphereCollider*& coll : attackCollRef) { coll = nullptr; }
	}

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 攻撃対象を探索
	void TargetSerch(void);

	// 攻撃対象が見つかったかどうかを取得
	bool GetIsFindAttackTarget(void)const { return isFindAttackTarget; }
	// 攻撃対象の座標を取得
	const Vector3& GetTargetPos(void)const { return *targetPos; }

	// 指定の段の攻撃の判定を発生させる
	void CollOn(PLAYER_TRIPLE_ATTACK_STAGE stage, bool sendNetwork = true);

	// 指定の段の攻撃の判定を消す（指定がない場合は全ての段の判定を消す）
	void CollOff(PLAYER_TRIPLE_ATTACK_STAGE stage = PLAYER_TRIPLE_ATTACK_STAGE::None, bool sendNetwork = true);

	// 攻撃対象をリセットする
	void ResetTarget(void) { isFindAttackTarget = false; targetPos = nullptr; }

	// 攻撃のヒット管理のフラグをリセットする
	void ResetIsHit(void) { isHit = false; }

private:

#pragma region 定数

	// 攻撃対象を見つける距離
	const float FIND_ATTACK_TARGET_RANGE;

	// 攻撃のタグ
	COLLIDER_TAG COLL_TAG;

	// 攻撃判定のサイズ（半径）
	const float COLL_SIZE;

	// 攻撃の段階ごとの判定のローカル座標
	const Vector3 COLL_LOCAL_POS;

	// 三段攻撃の攻撃倍率
	const std::array<short, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max> ATTACK_RATE_PERCENT;

	// 攻撃を送信する際のタイプ
	const std::array<MsgDataPlayerCollOperator::COLLIDER_TYPE, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max> SEND_COLLIDER_TYPE;

#pragma endregion

#pragma region 受け取る参照変数

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 操作権の参照
	const MSG_SENDER_ID& controlSenderId;

	// プレイヤーのステータス
	const CharacterStats& playerStats;

#pragma endregion

	// 探索用のコライダーの参照用
	SphereCollider* findCollRef;
	// 攻撃用のコライダーの参照用
	SphereCollider* attackCollRef[(int)PLAYER_TRIPLE_ATTACK_STAGE::Max];

	// 攻撃対象が見つかったかどうか
	bool isFindAttackTarget;

	// 攻撃のヒット管理のフラグ
	bool isHit;

	// 攻撃対象の座標
	const Vector3* targetPos;

	// 更新処理
	void SubUpdate(void)override;

	// 現在の判定の有無
	bool IsCollActive(void)const;
};