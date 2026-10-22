#include "TomatoPlayerStampState.h"

#include "../../../../../../../Manager/Input/InputManager.h"
#include "../../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../../Manager/Camera/CurrentCamera.h"
#include "../../../../../../../Manager/TimeScale/TimeScale.h"

#include "../../../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackCollOperator.h"

TomatoPlayerStampState::TomatoPlayerStampState(
	const bool& isGround,

	PlayerSimpleAttackCollOperator& collOperator,

	const std::function<void(const Vector3&)>& moveAccel,
	float& accelMax,

	Vector3& velocity,

	const std::function<void(bool)>& setIsGravity,
	const std::function<void(void)>& defaultChangeState
) :
	DEFAULT_ACCEL_MAX(accelMax),

	isGround(isGround),

	collOperator(collOperator),

	moveAccel(moveAccel),
	accelMax(accelMax),

	velocity(velocity),
	setIsGravity(setIsGravity),
	defaultChangeState(defaultChangeState)
{
}

void TomatoPlayerStampState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerSkill3).down) {
		OwnChangeState();
	}
}

void TomatoPlayerStampState::Enter(void)
{
	// 重力を一時的に消す
	setIsGravity(false);

	// クールタイムをセット
	coolTimeCounter = COOL_TIME;

	// ジャンプの初速を設定
	velocity.y = JUMP_POWER;

	// フラグの初期化
	isEnd = false;
	collOperator.ResetIsHit();

	accelMax = JUMP_MOVE_SPEED;
}

void TomatoPlayerStampState::Update(void)
{
	// 攻撃終了フラグがtrueの場合ステートを抜ける
	if (isEnd) { defaultChangeState(); return; }

	// 地面に到着したら、当たり判定を発生
	if (isGround) {
		collOperator.CollOn();

		// 1フレーム待ってステートを終了する
		isEnd = true;
	}

	// 重力をかけていく
	velocity.y -= GRAVITY;

	// 移動処理
	JumpMove();
}

void TomatoPlayerStampState::Exit(void)
{
	// すべて一応初期化
	collOperator.CollOff();
	collOperator.ResetIsHit();
	isEnd = false;

	accelMax = DEFAULT_ACCEL_MAX;
	// 重力をもとに戻す
	setIsGravity(true);
}

void TomatoPlayerStampState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0.0f) {
			coolTimeCounter -= TimeScale::Get();
			if (coolTimeCounter < 0.0f) { coolTimeCounter = 0.0f; }
		}
	}
}

void TomatoPlayerStampState::JumpMove(void)
{
	//コントローラーの入力を取得
	Vector3 vec = Input::GetIns().GetLeftStickVec().ToVector3XZ();

	// 入力がなければ次にキーボードの入力を取得
	if (vec == 0.0f) {
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
	}

	// 最終的に入力があれば加速度に加算する
	if (vec != 0.0f) {

		// 正規化
		vec.Normalize();

		// 移動方向をカメラで回転させる
		vec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetRotation().GetYaw()));

		// 移動
		moveAccel(vec);
	}
}
