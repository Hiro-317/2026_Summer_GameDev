#include "GrapePlayerThrowState.h"

GrapePlayerThrowState::GrapePlayerThrowState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	GrapePlayerThrowCollOperator& collOperator,
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
	moveVec(Vector3()),
	isThrow(false)
{
}

void GrapePlayerThrowState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Input::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_2).down) {
		OwnChangeState();
	}
}

void GrapePlayerThrowState::Enter(void)
{
	coolTimeCounter = COOL_TIME;

	PlayAnime();

	isThrow = false;
}

void GrapePlayerThrowState::Update(void)
{
	// 突進方向の取得
	moveVec = rotation.Rotate(Vector3(0.0f, 0.0f, 1.0f));
	// ボスへの方向を取得
	Vector3 bossVec = (*bossPos - pos).Normalized();

	// 内積をとってなす角を取得する
	float dot = moveVec.Dot(bossVec);

	// 一定範囲内なら、ボスの方を向かせる補正をかける
	if (dot > Deg2Rad(30.0f)) { rotation = Quaternion::FromRotationY(atan2f(bossVec.x, bossVec.z)); }

	if (AnimeRatio() < 0.3f) {
		if (!isThrow) {
			collOperator.LocalThrowBombStart(pos, moveVec);
			isThrow = true;
		}
	}

	if (IsAnimeEnd()) {
		DefaultChangeState();
	}
}

void GrapePlayerThrowState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}
