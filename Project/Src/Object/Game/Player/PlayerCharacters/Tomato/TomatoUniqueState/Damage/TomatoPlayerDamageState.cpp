#include "TomatoPlayerDamageState.h"

#include "../../../../../../../Utility/Utility.h"

#include "../../../../../../../Common/Quaternion.h"

TomatoPlayerDamageState::TomatoPlayerDamageState(Quaternion& rotation, const std::function<void(void)> inviStart, const std::function<void(void)> defaultChangeState):
	rotation(rotation),
	inviStart(inviStart),
	defaultChangeState(defaultChangeState)
{
}

// 状態遷移後1度行う初期化処理
void TomatoPlayerDamageState::Enter(void)
{
	// 無敵カウンターを設定
	inviStart();

	rotation = Quaternion::FromRotationX(Deg2Rad(180.0f));
}

// 更新処理
void TomatoPlayerDamageState::Update(void)
{
	// 回転
	rotation.AddAngle(Vector3::Yonly(Deg2Rad(0.5f)));

	// 元の状態に遷移
	if (rotation.GetYaw() > 20.0f) { defaultChangeState(); }
}

// 状態遷移前1度行う終了処理
void TomatoPlayerDamageState::Exit(void)
{
	rotation = Quaternion();
}
