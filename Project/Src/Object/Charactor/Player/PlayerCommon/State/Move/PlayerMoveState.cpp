#include "PlayerMoveState.h"

#include "../../../../../../Manager/Input/KeyManager.h"
#include "../../../../../../Manager/Camera/Camera.h"

PlayerMoveState::PlayerMoveState(
	const std::function<void(void)>& ownChangeState,
	std::function<bool(void)> isOwnState,
	Vector3& accelSum, Vector3& angle,
	float ADD_MOVE_SPEED, float DASH_SPEED_RATE, short DASH_STAMINA_MAX,
	const std::function<void(void)> PlayIdleAnime,
	const std::function<void(void)> PlayWalkAnime,
	const std::function<void(void)> PlayRunAnime
) :
	CharactorStateBase(ownChangeState, isOwnState),
	accelSum(accelSum), angle(angle),
	ADD_MOVE_SPEED(ADD_MOVE_SPEED),	DASH_SPEED_RATE(DASH_SPEED_RATE), DASH_STAMINA_MAX(DASH_STAMINA_MAX),
	PlayIdleAnime(PlayIdleAnime),
	PlayWalkAnime(PlayWalkAnime),
	PlayRunAnime(PlayRunAnime),

	dashFlg(false), dashStamina(0)
{
}

void PlayerMoveState::OwnStateConditionUpdate(void)
{
	// 移動にかかわるキーのダウントリガーを条件にする
	if (
		Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_RIGHT).down ||
		Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_LEFT).down ||
		Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_FRONT).down ||
		Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_BACK).down ||
		Key::GetIns().GetInfo(KEY_TYPE::PLAYER_DASH).down
		) {
		// 自分の状態に遷移
		ownChangeState();
	}
}

void PlayerMoveState::Init(void)
{
	accelSum = 0.0f;
	dashFlg = false;
	dashStamina = DASH_STAMINA_MAX;
}

void PlayerMoveState::Update(void)
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

	// ダッシュフラグを更新
	dashFlg = (dashStamina <= 0)? false : Key::GetIns().GetInfo(KEY_TYPE::PLAYER_DASH).now;

	// 最終的に入力があれば加速度に加算する
	if (vec != 0.0f) { 

		// 正規化
		vec.Normalize();

		// 移動方向をカメラで回転させる
		vec.TransMatOwn(MGetRotY(Camera::GetIns().GetAngle().y));

		// 移動量を加算
		accelSum += vec * (ADD_MOVE_SPEED * (dashFlg ? DASH_SPEED_RATE : 1.0f));

		// ダッシュスタミナを更新 / アニメーションを更新
		if (dashFlg) {
			if (--dashStamina < 0) { dashStamina = 0; }
			PlayRunAnime();
		}
		else {
			if (++dashStamina > DASH_STAMINA_MAX) { dashStamina = DASH_STAMINA_MAX; }
			PlayWalkAnime();
		}
	}
	else {
		// 移動入力がなければダッシュスタミナを回復させて待機アニメーションにする
		if (++dashStamina > DASH_STAMINA_MAX) { dashStamina = DASH_STAMINA_MAX; }
		PlayIdleAnime();
	}

	// 角度を入力方向に向ける
	if (vec != 0.0f) { angle.y = atan2f(vec.x, vec.z); }
}

void PlayerMoveState::Exit(void)
{
}
