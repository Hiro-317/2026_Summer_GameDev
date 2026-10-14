#include "BossSelectSkyShipMoveState.h"

#include "../../../../Common/Vector3.h"

#include "../../../../Manager/Input/InputManager.h"

#include "../../../../Manager/Camera/CurrentCamera.h"

BossSelectSkyShipMoveState::BossSelectSkyShipMoveState(std::function<void(const Vector3&)> moveAccel) :
	CharacterStateBase(),

	moveAccel(moveAccel)
{
}

void BossSelectSkyShipMoveState::Enter(void)
{

}

void BossSelectSkyShipMoveState::Update(void)
{
	// コントローラーの入力を取得
	Vector3 vec = Input::GetIns().GetLeftStickVec(false).ToVector3XZ();

	// 入力がなければ次にキーボードの入力を取得
	if (vec == 0.0f) {
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
		vec.Normalize();
	}

	// 最終的に入力があれば加速度に加算する
	if (vec != 0.0f) {

		// 移動方向をカメラで回転させる
		vec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

		// 移動
		moveAccel(vec);
	}
}

void BossSelectSkyShipMoveState::Exit(void)
{

}