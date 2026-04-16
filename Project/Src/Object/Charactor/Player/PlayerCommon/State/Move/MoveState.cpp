#include "MoveState.h"

#include "../../../../../../Manager/Input/KeyManager.h"

MoveState::MoveState(Vector3& accelSum, float ADD_ACCEL) :
	accelSum(accelSum),
	ADD_ACCEL(ADD_ACCEL)
{
}

void MoveState::Init(void)
{
	accelSum = 0.0f;
}

void MoveState::Update(void)
{
	// コントローラーの入力を取得
	Vector3 vec = Key::GetIns().GetLeftStickVec().ToVector3XZ();

	// 入力がなければ次にキーボードの入力を取得
	if (vec == 0.0f) {
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_RIGHT).now) { vec.x++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_LEFT).now) { vec.x--; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_FRONT).now) { vec.z++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_BACK).now) { vec.z--; }
	}

	// 最終的に入力があれば加速度に加算する
	if (vec != 0.0f) { accelSum += vec.Normalized() * ADD_ACCEL; }
}