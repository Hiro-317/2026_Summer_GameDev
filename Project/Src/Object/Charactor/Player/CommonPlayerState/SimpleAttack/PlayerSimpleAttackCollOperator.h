#pragma once

#include "../../../../ActorBase.h"

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
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="playerAngle">プレイヤーの向き</param>
	PlayerSimpleAttackCollOperator(
		float FIND_ATTACK_TARGET_RANGE,
		COLL_TAG COLL_TAG, float COLL_SIZE,
		const Vector3& COLL_LOCAL_POS,

		const Vector3& playerPos,
		const Vector3& playerAngle
	);
	~PlayerSimpleAttackCollOperator()override = default;

	void Load(void)override;

	void OnCollision(const ColliderBase& other)override;

	// 攻撃対象を探索
	void TargetSerch(void) {
		ColliderSerch(COLL_TAG::PLAYER_TRIPLE_ATTACK_TARGET_SERCH).back()->SetJudgeFlg(true);
	}

	// 攻撃対象が見つかったかどうかを取得
	bool GetIsFindAttackTarget(void)const { return isFindAttackTarget; }
	// 攻撃対象の座標を取得
	const Vector3& GetTargetPos(void)const { return *targetPos; }

	// 攻撃の判定を発生させる
	void CollOn(void) { ColliderSerch(COLL_TAG).back()->SetJudgeFlg(true); }

	// 攻撃の判定を消す
	void CollOff(void) { ColliderSerch(COLL_TAG).back()->SetJudgeFlg(false); }

	// 攻撃対象をリセットする
	void ResetTarget(void) { isFindAttackTarget = false; targetPos = nullptr; }
private:

#pragma region 定数

	// 攻撃対象を見つける距離
	const float FIND_ATTACK_TARGET_RANGE;

	// 攻撃のタグ
	const COLL_TAG COLL_TAG;

	// 攻撃判定のサイズ（半径）
	const float COLL_SIZE;

	// 攻撃判定のローカル座標
	const Vector3 COLL_LOCAL_POS;

#pragma endregion

#pragma region 受け取る参照変数

	// プレイヤーの座標
	const Vector3& playerPos;
	// プレイヤーの向き
	const Vector3& playerAngle;

#pragma endregion

	// 攻撃対象が見つかったかどうか
	bool isFindAttackTarget;

	// 攻撃対象の座標
	const Vector3* targetPos;

	// 更新処理
	void SubUpdate(void)override;
};