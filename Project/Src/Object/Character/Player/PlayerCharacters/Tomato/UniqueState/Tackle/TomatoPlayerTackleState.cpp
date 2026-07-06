#include "TomatoPlayerTackleState.h"

#include "../../../../../../../Manager/Input/KeyManager.h"
#include "../../../../../../../Manager/Sound/SoundManager.h"

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
	//	当たったかどうかのフラグをリセット
	collOperator.ResetIsHit();

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
	// カウンターがゼロになったら終了
	if (timeCounter <= 0) {
		DefaultChangeState();
		return;
	}

	// 一定数回転したら突進を開始
	if (coolTimeCounter >= COOL_TIME) {
		SoundManager::GetIns().Stop("TackleCharge");
		SoundManager::GetIns().Play("TackleMove");

		// 突進中も回転を続ける
		angle.x += ROTATION_POW;
		// カウントを開始
		timeCounter--;
		// 前方に向かって突進
		pos += moveDir * MOVE_SPEED;

		// 当たり判定をオンにする
		collOperator.CollOn();

		// 当たったらタイマーを強制的に終了し、次の処理へ
		if (collOperator.GetIsHit()) {
			timeCounter = 0;
			collOperator.CollOff();
		}
	}

	// ボタンを押し続けている場合攻撃をチャージする
	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_2).now) {
		// これはスキルのUIをチャージ用に動かすための特殊処理処理
		Charge(coolTimeCounter, COOL_TIME, 60);

		// 回転スタート
		angle.x += ROTATION_POW;
	
		SoundManager::GetIns().Play("TackleCharge");
	}
	else if (coolTimeCounter < COOL_TIME) {
		SoundManager::GetIns().Stop("TackleCharge");
		// チャージが終了しなかったら、強制的に終了させる
		DefaultChangeState();
	}
}

void TomatoPlayerTackleState::Exit(void)
{
	// 念のためすべて初期化
	timeCounter = 0;
	collOperator.CollOff();
	collOperator.ResetIsHit();

	// スキルのクールタイムをセット
	coolTimeCounter = COOL_TIME;
}

void TomatoPlayerTackleState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}
