#include "PlayerMoveState.h"

#include "../../../../../Manager/Input/InputManager.h"
#include "../../../../../Manager/Camera/CurrentCamera.h"
#include "../../../../Common/CharacterBase/CharacterStatsDefine.h"

PlayerMoveState::PlayerMoveState(
	const CharacterStats& stats,
	float DASH_SPEED_RATE,
	short DASH_STAMINA_MAX,

	std::function<void(const Vector3&)> moveAccel,
	float& ACCEL_MAX,

	std::function<void(void)> playAnimeWalk,
	std::function<void(void)> playAnimeRun
) :
	stats(stats), DASH_SPEED_RATE(DASH_SPEED_RATE), DASH_STAMINA_MAX(DASH_STAMINA_MAX),

	moveAccel(moveAccel),
	ACCEL_MAX(ACCEL_MAX),

	playAnimeWalk(playAnimeWalk),
	playAnimeRun(playAnimeRun),

	isDash(false), dashStamina(DASH_STAMINA_MAX), isTired(false)
{
}

void PlayerMoveState::OwnStateConditionUpdate(void)
{
	// 移動にかかわるキーのダウントリガーを条件にする
	if (
		Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).down ||
		Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).down ||
		Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).down ||
		Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).down ||
		Input::GetIns().GetInfo(KEY_TYPE::PlayerDash).down
		) {
		// 自分の状態に遷移
		OwnChangeState();
	}
}

void PlayerMoveState::Enter(void)
{
}

void PlayerMoveState::Update(void)
{
	// 移動方向入力を取得
	Vector3 inputVec = InputVec();

	// ダッシュフラグを立てる(移動中じゃなければ、ダッシュフラグは変えない)
	isDash = (isTired) ? false : Input::GetIns().GetInfo(KEY_TYPE::PlayerDash).now && inputVec != 0.0f;

	// 移動量の最大値を更新する
	ACCEL_MAX = MOVE_SPEED_MAX(isDash);

	// 最終的に入力があれば加速度に加算する
	if (inputVec != 0.0f) {

		// 移動方向をカメラで回転させる
		inputVec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetRotation().GetYaw()));

		// 移動
		moveAccel(inputVec);

		// ダッシュスタミナを更新 / アニメーションを更新
		if (isDash) {

			// ダッシュしているときはスタミナを減らす
			if (--dashStamina < 0) {
				dashStamina = 0;

				// 息切れ
				isTired = true;
			}

			// ダッシュしているときは走るアニメーションにする
			playAnimeRun();
		}
		else {
			// ダッシュしていないときは歩くアニメーションにする
			playAnimeWalk();
		}
	}
}

void PlayerMoveState::Exit(void)
{
}

void PlayerMoveState::AlwaysUpdate(void)
{
	// ダッシュしていないときはスタミナを回復させる
	if (!isDash) {
		if (++dashStamina > DASH_STAMINA_MAX) {
			dashStamina = DASH_STAMINA_MAX;

			// 息切れ回復
			isTired = false;
		}
	}
}

Vector3 PlayerMoveState::InputVec(void) const
{
	// 返却用一時変数
	Vector3 vec = Vector3();

	// コントローラーの入力を取得
	vec = Input::GetIns().GetLeftStickVec(false).ToVector3XZInvertY();

	// 入力がなければ次にキーボードの入力を取得
	if (vec == 0.0f) {
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
		vec.Normalize();
	}

	return vec;
}

float PlayerMoveState::MOVE_SPEED(bool isDash)const 
{
	return (stats.speedPower.Value() * 0.5f) * (isDash ? DASH_SPEED_RATE : 1.0f);
}
float PlayerMoveState::MOVE_SPEED_MAX(bool isDash)const 
{
	return stats.speedPower.Value() * (isDash ? DASH_SPEED_RATE : 1.0f);
}