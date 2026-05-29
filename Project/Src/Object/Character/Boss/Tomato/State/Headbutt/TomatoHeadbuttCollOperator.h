#pragma once

#include "../../../../../ActorBase.h"

class TomatoHeadbuttCollOperator : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	TomatoHeadbuttCollOperator(
		const float TO_PLAYER_DISTANCE,
		const CharacterStats stats,
		const ParameterLoad& collParam
	);
	~TomatoHeadbuttCollOperator()override = default;

	void Load(void)override;

	void CollSet(bool flg) { SetJudge(flg); }

	void SetPos(Vector3 pos) { trans.pos = pos; }

private:

	// カプセルコライダーのローカル座標
	const float TO_PLAYER_DISTANCE;


#pragma region 受け取る参照変数

	const CharacterStats stats;

	const ParameterLoad& collParam;

#pragma endregion

};