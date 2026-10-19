#include "TomatoPlayerHeadButtState.h"

#include "../../../../../../../Manager/Input/InputManager.h"


TomatoPlayerHeadButtState::TomatoPlayerHeadButtState(
	int COOL_TIME,
	float ATTACK_TIME,
	float ATTACK_SPEED,

	PlayerSimpleAttackCollOperator& collOperator,

	Transform& playerTrans,

	std::function<void(void)> defaultChangeState
):
	COOL_TIME(COOL_TIME),
	ATTACK_TIME(ATTACK_TIME),
	ATTACK_SPEED(ATTACK_SPEED),
	
	collOperator(collOperator),

	playerTrans(playerTrans),

	defaultChangeState(defaultChangeState)
{
}

void TomatoPlayerHeadButtState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_1).down) {
		OwnChangeState();
	}
}

void TomatoPlayerHeadButtState::Enter(void)
{
	// 向いている方向を取得
	moveDir.x = sinf(angle.y);
	moveDir.z = cosf(angle.y);
	moveDir.Normalize();
	
	// トマトを水平にリセット
	angle.x = 0.0f;
	// 開始時のY座標を取得
	startY = pos.y;
	// タイマーを開始
	timeCounter = 0.0f;
	// クールタイムをセット
	coolTimeCounter = COOL_TIME;
}

void TomatoPlayerHeadButtState::Update(void)
{	
	// タイマーを開始
	timeCounter++;

	// タイマーがATTACK_TIME未満の時は、頭突き攻撃をする
	if (timeCounter < ATTACK_TIME) {
		// 位置の更新
		float cnt = timeCounter <= ATTACK_TIME / 2.0f ? timeCounter : timeCounter - (ATTACK_TIME + 0.5f);
		pos += moveDir * ATTACK_SPEED * cnt;
		pos.y = startY + 30.0f;

		// 当たり判定を開始
		collOperator.CollOn();

		// ボスに当たったので、当たり判定を消す
		if (collOperator.GetIsHit()) {
			collOperator.CollOff();
		}
	}
	else {
		// タイマーが終了したので攻撃状態を終了する
		DefaultChangeState();
	}
}

void TomatoPlayerHeadButtState::Exit(void)
{
	// すべて初期化
	collOperator.CollOff();
	collOperator.ResetIsHit();
}

void TomatoPlayerHeadButtState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}
