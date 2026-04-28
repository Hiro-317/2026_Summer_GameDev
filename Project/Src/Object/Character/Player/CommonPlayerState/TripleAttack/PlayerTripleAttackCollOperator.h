#pragma once

#include "../../../../ActorBase.h"

#include "PlayerTripleAttackStDefine.h"

class PlayerTripleAttackCollOperator : public ActorBase
{
public:

	/// <summary>
	/// コンストラクト
	/// </summary>
	/// <param name="FIND_ATTACK_TARGET_RANGE">攻撃対象の探索範囲</param>
	/// <param name="COLL_TAG"></param>
	/// <param name="COLL_SIZE"></param>
	/// <param name="COLL_LOCAL_POS">攻撃判定の座標（プレイヤー座標からのローカル座標）</param>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="playerAngle">プレイヤーの角度</param>
	PlayerTripleAttackCollOperator(
		float FIND_ATTACK_TARGET_RANGE,
		const std::array<COLLIDER_TAG, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_TAG,
		const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_SIZE,
		const Vector3& COLL_LOCAL_POS,

		const Vector3& playerPos, const Vector3& playerAngle
	);
	~PlayerTripleAttackCollOperator()override = default;

	void Load(void)override;

	void OnCollision(const ColliderBase& other)override;

	// 攻撃対象を探索
	void TargetSerch(void) {
		ColliderSerch(COLLIDER_TAG::PLAYER_TRIPLE_ATTACK_TARGET_SERCH).back()->SetJudgeFlg(true); 
	}

	// 攻撃対象が見つかったかどうかを取得
	bool GetIsFindAttackTarget(void)const { return isFindAttackTarget; }
	// 攻撃対象の座標を取得
	const Vector3& GetTargetPos(void)const { return *targetPos; }

	// 指定の段の攻撃の判定を発生させる
	void CollOn(PLAYER_TRIPLE_ATTACK_STAGE stage) { ColliderSerch(COLL_TAG[(int)stage]).back()->SetJudgeFlg(true); }

	// 指定の段の攻撃の判定を消す（指定がない場合は全ての段の判定を消す）
	void CollOff(PLAYER_TRIPLE_ATTACK_STAGE stage = PLAYER_TRIPLE_ATTACK_STAGE::NON) {
		if (stage == PLAYER_TRIPLE_ATTACK_STAGE::NON) {
			// 指定がない場合は全ての段の判定を消す
			for (int i = 0; i < (int)PLAYER_TRIPLE_ATTACK_STAGE::MAX; i++) {
				ColliderSerch(COLL_TAG[i]).back()->SetJudgeFlg(false);
			}
		}
		// 指定がある場合はその段の判定を消す
		else { ColliderSerch(COLL_TAG[(int)stage]).back()->SetJudgeFlg(false); }
	}

	// 攻撃対象をリセットする
	void ResetTarget(void) { isFindAttackTarget = false; targetPos = nullptr; }

private:

#pragma region 定数

	// 攻撃対象を見つける距離
	const float FIND_ATTACK_TARGET_RANGE;

	// 攻撃の段階ごとのタグ
	const std::array<COLLIDER_TAG, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_TAG;

	// 攻撃の段階ごとの判定のサイズ（半径）
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_SIZE;

	// 攻撃の段階ごとの判定のローカル座標
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