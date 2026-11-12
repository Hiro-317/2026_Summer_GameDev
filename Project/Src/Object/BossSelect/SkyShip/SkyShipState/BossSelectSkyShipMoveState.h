#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;
struct Quaternion;

class BossSelectSkyShipMoveState : public CharacterStateBase
{
public:

	BossSelectSkyShipMoveState(std::function<void(const Vector3&)> moveAccel);

	~BossSelectSkyShipMoveState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 更新処理
	void Update(void)override;

	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:
	// 移動関数のポインタ
	const std::function<void(const Vector3&)> moveAccel;
};