#include "GrapePlayerShotState.h"

#include <cmath>

#include "../../../../../../../Utility/Utility.h"

#include "../../../../../../../Manager/Input/InputManager.h"
#include "../../../../../../../Manager/TimeScale/TimeScale.h"

#include "GrapePlayerWeaponShot.h"

GrapePlayerShotState::GrapePlayerShotState(
	const int COOL_TIME,

	GrapePlayerWeaponShot& shotWeapon,

	Transform& playerTrans,

	const Vector3*& bossPos,

	const std::function<void(void)> playAnime,
	const std::function<float(void)> getAnimeRatio,
	const std::function<void(void)> defaultChangeState
) :
	COOL_TIME(COOL_TIME),
	shotWeapon(shotWeapon),
	playerTrans(playerTrans),
	bossPos(bossPos),
	playAnime(playAnime),
	getAnimeRatio(getAnimeRatio),
	defaultChangeState(defaultChangeState),

	moveDir(),
	isShot(),
	coolTimeCounter()
{
}

void GrapePlayerShotState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0.0f) { return; }

	// ステート遷移処理
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerSkill1).down) {
		OwnChangeState();
	}
}

void GrapePlayerShotState::Enter(void)
{
	coolTimeCounter = (float)COOL_TIME;

	playAnime();

	isShot = false;
}

void GrapePlayerShotState::Aim(void)
{
    // 正面とボス方向は水平面で比較する。背後や範囲外には補正しない。
    moveDir = playerTrans.rotation.Rotate(Vector3::Zonly(1.0f));
    moveDir.y = 0.0f;
    if (moveDir.LengthSq() <= 0.000001f) { moveDir = Vector3::Zonly(1.0f); }
    moveDir.Normalize();
    if (bossPos == nullptr) { return; }

    Vector3 bossDirection = *bossPos - playerTrans.pos;
    bossDirection.y = 0.0f;
    if (bossDirection.LengthSq() <= 0.000001f) { return; }
    bossDirection.Normalize();

    const float assistCos = std::cos(Deg2Rad(30.0f));
    if (moveDir.Dot(bossDirection) >= assistCos) {
        moveDir = bossDirection;
        playerTrans.SetRotation(Quaternion::FromRotationY(std::atan2(moveDir.x, moveDir.z)));
    }
}

void GrapePlayerShotState::Update(void)
{
    const float animeRatio = getAnimeRatio();
    if (!isShot && animeRatio >= 0.3f) {
        // 補助は発射時に一度だけ判定し、その時点の方向を武器へ渡す。
        // 投擲後にプレイヤーやボスが動いても、弾の進行方向は変わらない。
        Aim();
        shotWeapon.Shot(playerTrans.pos, moveDir, true);
        isShot = true;
    }
    if (animeRatio >= 1.0f) { defaultChangeState(); }
}

void GrapePlayerShotState::AlwaysUpdate(void)
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
