#include "TomatoPlayerHeadButtState.h"

#include <algorithm>

#include "../../../../../../Common/Transform/Transform.h"

#include "../../../../../../../Manager/Input/InputManager.h"
#include "../../../../../../../Manager/TimeScale/TimeScale.h"

#include "../../../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackCollOperator.h"

TomatoPlayerHeadButtState::TomatoPlayerHeadButtState(
	int COOL_TIME,

	float COLL_START_TIME,
	float COLL_END_TIME,
	float STATE_END_TIME,

	PlayerSimpleAttackCollOperator& collOperator,

	Transform& playerTrans,

	std::function<void(void)> defaultChangeState
) :
	COOL_TIME(COOL_TIME),

	COLL_START_TIME(COLL_START_TIME),
	COLL_END_TIME(COLL_END_TIME),
	STATE_END_TIME(STATE_END_TIME),

	collOperator(collOperator),

	playerTrans(playerTrans),
	DEFAULT_CENTER_DIFF(playerTrans.centerDiff),

	defaultChangeState(defaultChangeState),

	time(0.0f),
	coolTimeCounter()
{
}

void TomatoPlayerHeadButtState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerSkill1).down) {
		OwnChangeState();
	}
}

void TomatoPlayerHeadButtState::Enter(void)
{
	// 当たり判定の初期化
	collOperator.CollOff();
	collOperator.ResetIsHit();

	// タイマーを初期化
	time = 0.0f;
	collActive = false;
	playerTrans.centerDiff = DEFAULT_CENTER_DIFF;

	// クールタイムをセット
	coolTimeCounter = COOL_TIME;
}

void TomatoPlayerHeadButtState::Update(void)
{
	// 経過時間を更新
	time += TimeScale::Get();

	// 攻撃判定を指定時間内だけ有効にする
	const bool shouldCollide = (time >= COLL_START_TIME && time < COLL_END_TIME);
	if (shouldCollide && !collActive) {
		collOperator.CollOn();
		collActive = true;
	}
	else if (!shouldCollide && collActive) {
		collOperator.CollOff();
		collActive = false;
	}

	// ステート全体に対する割合で、予備動作 → 突進 → 戻りを制御
	const float windupEnd = STATE_END_TIME * 0.15f;
	const float thrustEnd = STATE_END_TIME * 0.35f;
	const float returnEnd = STATE_END_TIME * 0.70f;

	float distance = 0.0f;
	if (time < windupEnd && windupEnd > 0.0f) {
		const float t = std::clamp(time / windupEnd, 0.0f, 1.0f);
		const float smooth = t * t * (3.0f - 2.0f * t);
		distance = -HEADBUTT_WINDUP * smooth;
	}
	else if (time < thrustEnd && thrustEnd > windupEnd) {
		const float t = std::clamp((time - windupEnd) / (thrustEnd - windupEnd), 0.0f, 1.0f);
		const float easeOut = 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
		distance = -HEADBUTT_WINDUP + (HEADBUTT_DISTANCE + HEADBUTT_WINDUP) * easeOut;
	}
	else if (time < returnEnd && returnEnd > thrustEnd) {
		const float t = std::clamp((time - thrustEnd) / (returnEnd - thrustEnd), 0.0f, 1.0f);
		const float smooth = t * t * (3.0f - 2.0f * t);
		distance = HEADBUTT_DISTANCE * (1.0f - smooth);
	}

	// Transform::Attach() 側で centerDiff は FinalRotationMat() により回転する。
	// そのため、ワールド方向へ変換せず、モデルのローカル +Z に加算する。
	playerTrans.centerDiff = DEFAULT_CENTER_DIFF + Vector3(0.0f, 0.0f, distance);

	if (time >= STATE_END_TIME) {
		playerTrans.centerDiff = DEFAULT_CENTER_DIFF;
		defaultChangeState();
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
		if (coolTimeCounter > 0) { coolTimeCounter -= TimeScale::Get(); }
	}
}
