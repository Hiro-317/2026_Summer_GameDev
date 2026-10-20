#include "GrapePlayerShotState.h"

GrapePlayerShotState::GrapePlayerShotState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	GrapePlayerShotCollOperator& collOperator,
	const int COOL_TIME,
	Vector3& pos, Quaternion& rotation,
	const Vector3*& bossPos,
	const std::function<void(void)> PlayAnime,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<float(void)> AnimeRatio,
	const std::function<void(void)> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator),
	COOL_TIME(COOL_TIME),
	pos(pos), rotation(rotation),
	bossPos(bossPos),
	PlayAnime(PlayAnime),
	IsAnimeEnd(IsAnimeEnd),
	AnimeRatio(AnimeRatio),
	DefaultChangeState(DefaultChangeState),
	timeCounter(0),
	moveDir(Vector3()),
	isShot(false)
{
}

void GrapePlayerShotState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Input::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_1).down) {
		OwnChangeState();
	}
}

void GrapePlayerShotState::Enter(void)
{
	coolTimeCounter = COOL_TIME;

	PlayAnime();

	isShot = false;
}

void GrapePlayerShotState::Update(void)
{
	if (AnimeRatio() < 0.3f) {
		// 発射の瞬間までじわじわボスの方を向かせる

		// プレイヤーの前方方向を取得
		moveDir = rotation.Rotate(Vector3(0.0f, 0.0f, 1.0f));
		// ボスへの方向を取得
		Vector3 bossVec = (*bossPos - pos).Normalized();
		// 内積をとってなす角を取得する
		float dot = moveDir.Dot(bossVec);
		// 一定範囲内なら、ボスの方を向かせる補正をかける
		if (dot > Deg2Rad(30.0f)) { rotation = Quaternion::FromRotationY(atan2f(bossVec.x, bossVec.z)); }
	}
	else {
		if (!isShot) {
			// 発射
			collOperator.LocalShotStart(pos, moveDir);
			isShot = true;
		}
	}

	if (IsAnimeEnd()) {
		DefaultChangeState();
	}
}

void GrapePlayerShotState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}
