#include "GrapePlayerThrowState.h"

#include "../../../../../../../Utility/Utility.h"

#include "../../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../../Manager/Input/InputManager.h"
#include "../../../../../../../Manager/TimeScale/TimeScale.h"

#include "GrapePlayerWeaponThrow.h"

GrapePlayerThrowState::GrapePlayerThrowState(
	const int COOL_TIME,

	GrapePlayerWeaponThrow& throwWeapon,

	Transform& playerTrans,

	const Vector3*& bossPos,

	const std::function<void(void)> playAnime,
	const std::function<float(void)> getAnimeRatio,
	const std::function<void(void)> defaultChangeState
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

void GrapePlayerThrowState::Update(void)
{
	const float animeRatio = getAnimeRatio();

	if (animeRatio < 0.3f) {
		// 発射の瞬間までじわじわボスの方を向かせる

		// プレイヤーの前方方向を取得
		moveDir = playerTrans.rotation.Rotate(Vector3(0.0f, 0.0f, 1.0f));
		// ボスへの方向を取得
		Vector3 bossVec = (*bossPos - playerTrans.pos).Normalized();
		// 内積をとってなす角を取得する
		float dot = moveDir.Dot(bossVec);
		// 一定範囲内なら、ボスの方を向かせる補正をかける
		if (dot > Deg2Rad(30.0f)) { playerTrans.rotation = Quaternion::FromRotationY(atan2f(bossVec.x, bossVec.z)); }
	}
	else {
		if (!isThrow) {
			// 発射
			throwWeapon.Shot(playerTrans.pos, moveDir);
			isThrow = true;
		}
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
