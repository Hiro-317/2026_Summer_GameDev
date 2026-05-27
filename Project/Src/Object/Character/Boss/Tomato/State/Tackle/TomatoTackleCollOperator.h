#pragma once

#include "../../../../../ActorBase.h"

class TomatoTackleCollOperator : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	TomatoTackleCollOperator(
		const CharacterStats stats,
		const float TO_PLAYER_DISTANCE,
		const Vector3& pos
	);
	~TomatoTackleCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	void CollSet(bool flg) { ColliderSerch(COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(flg); }

	bool GetStageHit(void) { return stageHit; }

private:

	// カプセルコライダーのローカル座標
	const float TO_PLAYER_DISTANCE;


#pragma region 受け取る参照変数

	const CharacterStats stats;

	const Vector3& pos;

#pragma endregion

	// 更新処理
	void SubUpdate(void)override;

	bool stageHit;
};