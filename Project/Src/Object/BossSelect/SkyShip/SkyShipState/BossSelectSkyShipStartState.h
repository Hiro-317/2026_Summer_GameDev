#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;
struct Quaternion;

class BossSelectSkyShipStartState : public CharacterStateBase
{
public:
	BossSelectSkyShipStartState(
		const Vector3& LOCAL_START_POS,

		int POS_MAX_FRAME,
		int ANGLE_MAX_FRAME,

		Vector3& pos,
		Quaternion& rotation,

		const std::function<void(void)> MoveChangeState
	);

	~BossSelectSkyShipStartState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 更新処理
	void Update(void)override;

	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

	enum class STATE {
		None = -1,

		PosMove,
		AngleMove,

		Max
	};

	const Vector3 LOCAL_START_POS;

	// 時間
	const int POS_MAX_FRAME;
	const int ANGLE_MAX_FRAME;

	Vector3 START_POS;
	Quaternion START_ROTATION;
	Vector3 END_POS;
	Quaternion END_ROTATION;

	// 座標参照
	Vector3& pos;

	// 角度の参照
	Quaternion& rotation;

	const std::function<void(void)> MoveChangeState;

	int currentFrame;

	STATE state;
};