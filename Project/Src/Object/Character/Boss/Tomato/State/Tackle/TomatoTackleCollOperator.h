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
		const float TO_PLAYER_DISTANCE
	);
	~TomatoTackleCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	void CollSet(bool flg) { ColliderSerch(COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(flg); }

	void Set(Vector3 pos) { trans.pos = pos; }

	bool GetStageHit(void) { return stageHit; }

private:

	// カプセルコライダーのローカル座標
	const float TO_PLAYER_DISTANCE;


#pragma region 受け取る参照変数

	const CharacterStats stats;


#pragma endregion

	bool stageHit;
};