#include "TomatoPlayerTackleState.h"

#include "../../../../../../../Manager/Input/KeyManager.h"

TomatoPlayerTackleState::TomatoPlayerTackleState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	TomatoPlayerTackleCollOperator& collOperator,
	int COOL_TIME,
	float MOVE_SPEED, float ROTATION_POW,
	Vector3& pos, Vector3& angle,
	const std::function<void(void)> DefaultChangeState
	) : 
	CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator),
	COOL_TIME(COOL_TIME),
	MOVE_SPEED(MOVE_SPEED),
	ROTATION_POW(ROTATION_POW),
	pos(pos), angle(angle),
	DefaultChangeState(DefaultChangeState),
	timeCounter(0)
{

}

void TomatoPlayerTackleState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_2).down) {
		OwnChangeState();
	}
}

void TomatoPlayerTackleState::Enter(void)
{
	collOperator.ResetIsHit();

	// スキルのクールタイムをセット
	coolTimeCounter = COOL_TIME;

	// 前方方向の取得
	moveDir.x = sinf(angle.y);
	moveDir.z = cosf(angle.y);
	moveDir.Normalize();

	// X軸の角度をゼロに初期化(回転量で突進の開始を判断しているから、初期化しておく)
	angle.x = 0.0f;

	// カウンターの初期化
	timeCounter = COUNT_MAX;
}

void TomatoPlayerTackleState::Update(void)
{
	// 回転スタート
	angle.x += ROTATION_POW;

	// 一定数回転したら突進を開始
	if (angle.x >= TACKLE_START_ANGLE) {
		// カウントを開始
		timeCounter--;
		// 前方に向かって突進
		pos += moveDir * MOVE_SPEED;

		// 当たり判定をオンにする
		collOperator.CollOn();

		// 当たったらタイマーを強制的に終了し、次の処理へ
		if (collOperator.GetIsHit()) {
			timeCounter = 0;
		}
	}

	// カウンターがゼロになったら終了
	if (timeCounter <= 0) {
		// すべて初期化
		timeCounter = 0;
		DefaultChangeState();
		collOperator.CollOff();
	}
}

void TomatoPlayerTackleState::Exit(void)
{
	// 念のためすべて初期化
	timeCounter = 0;
	collOperator.CollOff();
	collOperator.ResetIsHit();
}

void TomatoPlayerTackleState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}
