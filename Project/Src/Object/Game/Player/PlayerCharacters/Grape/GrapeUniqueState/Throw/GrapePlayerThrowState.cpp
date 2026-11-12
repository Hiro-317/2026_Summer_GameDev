#include "GrapePlayerThrowState.h"

#include <cmath>

#include "../../../../../../../Utility/Utility.h"

#include "../../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../../Manager/Input/InputManager.h"
#include "../../../../../../../Manager/TimeScale/TimeScale.h"

#include "../../../../../../Common/Transform/Transform.h"

#include "GrapePlayerWeaponThrow.h"

GrapePlayerThrowState::GrapePlayerThrowState(
	const int COOL_TIME,

	GrapePlayerWeaponThrow& throwWeapon,

	Transform& playerTrans,

	const Vector3*& bossPos,

	const std::function<void(void)>& playAnime,
	const std::function<float(void)>& getAnimeRatio,
	const std::function<void(void)>& defaultChangeState
) :

	COOL_TIME(COOL_TIME),
	throwWeapon(throwWeapon),
	playerTrans(playerTrans),
	bossPos(bossPos),
	playAnime(playAnime),
	getAnimeRatio(getAnimeRatio),
	defaultChangeState(defaultChangeState),

	moveDir(),
	isThrow(),
	coolTimeCounter()
{
}

void GrapePlayerThrowState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerSkill2).down) {
		OwnChangeState();
	}
}

void GrapePlayerThrowState::Enter(void)
{
	coolTimeCounter = (float)COOL_TIME;

	playAnime();

	isThrow = false;
}

void GrapePlayerThrowState::Aim(void)
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

void GrapePlayerThrowState::Update(void)
{
    const float animeRatio = getAnimeRatio();
    if (!isThrow && animeRatio >= 0.3f) {
        // 補助は発射時に一度だけ判定し、その時点の方向を武器へ渡す。
        // 投擲後にプレイヤーやボスが動いても、弾の進行方向は変わらない。
        Aim();
        throwWeapon.Shot(playerTrans.pos, moveDir, true);
        isThrow = true;
    }
    if (animeRatio >= 1.0f) { defaultChangeState(); }
}

void GrapePlayerThrowState::AlwaysUpdate(void)
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
