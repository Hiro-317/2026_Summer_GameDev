#pragma once

#include "../../../../../Common/CharacterBase/CharacterStateBase.h"

struct Transform;
struct Vector3;

class GrapeBossMoveState : public CharacterStateBase
{
public:

	GrapeBossMoveState(
		float MOVE_SPEED,

		const Vector3& bossPos,

		const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
		const int& targetNum,

		const std::function<void(const Vector3&)>& moveAccel,

		const std::function<void(void)>& playRunAnime,
		const std::function<void(void)>& kickDownChangeState
	);
	~GrapeBossMoveState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:

#pragma region 定数

	// 移動量
	const float MOVE_SPEED;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// モデル制御情報の参照
	const Vector3& bossPos;

	// プレイヤーの座標の読み取り
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max];
	const int& targetNum;

	// 移動関数
	const std::function<void(const Vector3&)> moveAccel;

	// アニメーション
	const std::function<void(void)> playRunAnime;

	// キックに移行するポインタ
	const std::function<void(void)> kickDownChangeState;

#pragma endregion
};