#include "PlayerPreventionState.h"

#include "../../../../../Manager/Input/KeyManager.h"
#include "../../../../../Manager/Camera/Camera.h"
#include "../../../../Character/CharacterStatsDefine.h"

PlayerPreventionState::PlayerPreventionState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	float DASH_SPEED_RATE, short DASH_STAMINA_MAX, float ATTENUATION,
	Vector3& accelSum, float& ACCEL_MAX, Vector3& angle, const CharacterStats& playerStats,
	const std::function<void(void)>& PlayIdleAnime,
	const std::function<void(void)>& PlayWalkAnime,
	const std::function<void(void)>& PlayRunAnime,
	PlayerPreventionCollOperator& collOperator
) :
	CharacterStateBase(ownChangeState, isOwnState),
	DASH_SPEED_RATE(DASH_SPEED_RATE), DASH_STAMINA_MAX(DASH_STAMINA_MAX), ATTENUATION(ATTENUATION),
	accelSum(accelSum), ACCEL_MAX(ACCEL_MAX), angle(angle), playerStats(playerStats),
	PlayIdleAnime(PlayIdleAnime),
	PlayWalkAnime(PlayWalkAnime),
	PlayRunAnime(PlayRunAnime),
	collOperator(collOperator),

	isDash(false), dashStamina(DASH_STAMINA_MAX), isTired(false)
{
}

void PlayerPreventionState::OwnStateConditionUpdate(void)
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
		OwnChangeState();
	}
}

void PlayerPreventionState::Enter(void)
{
}

void PlayerPreventionState::Update(void)
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

	// 移動量の最大値を更新する
	ACCEL_MAX = playerStats.speedPower.Value();

	// 最終的に入力があれば加速度に加算する
	if (vec != 0.0f) {

		// 正規化
		vec.Normalize();

		// 移動方向をカメラで回転させる
		vec.TransMatOwn(MGetRotY(Camera::GetIns().GetAngle().y));

		// 移動量を加算
		accelSum += vec * (ACCEL_MAX / 2);

		// ダッシュアニメーションにする
		PlayWalkAnime();

	}
	else {
		// 待機アニメーションにする
		PlayIdleAnime();
	}

	// 角度を入力方向に向ける
	if (vec != 0.0f) { angle.y = atan2f(vec.x, vec.z); }
}

void PlayerPreventionState::Exit(void)
{
	accelSum = 0.0f; // 加速度をリセット
}

