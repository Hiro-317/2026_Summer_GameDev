#include "GrapeBossIdleState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossIdleState::GrapeBossIdleState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	Vector3& pos, Vector3& angle,
	const std::vector<const Vector3*> playerPos,
	const std::function<int(void)> CoolTime,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayIdleAnim,
	const std::function<void(void)> PlayWalkAnim,
	const std::function<void(void)> MoveChangeState,
	const std::function<void(void)> KickDownChangeState,
	const std::function<void(void)> StraightChangeState,
	const std::function<void(void)> StampChangeState,
	const std::function<void(void)> SingleChangeState,
	const std::function<void(void)> StalkerChangeState,
	const std::function<void(void)> RandomChangeState
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), angle(angle),
	playerPos(playerPos),
	CoolTime(CoolTime),
	GetTarget(GetTarget),
	PlayIdleAnim(PlayIdleAnim),
	PlayWalkAnim(PlayWalkAnim),
	MoveChangeState(MoveChangeState),
	KickDownChangeState(KickDownChangeState),
	StraightChangeState(StraightChangeState),
	StampChangeState(StampChangeState),
	SingleChangeState(SingleChangeState),
	StalkerChangeState(StalkerChangeState),
	RandomChangeState(RandomChangeState)
{
}

void GrapeBossIdleState::Enter(void)
{
	cnt = CoolTime();
	target = GetTarget();
	PlayIdleAnim();
}

void GrapeBossIdleState::Update(void)
{
	if (cnt > 0) {
		cnt--;
		
		// プレイヤーとの角度を出す
		Vector3 tmp = *playerPos.at(target) - pos;
		float dir = atan2f(tmp.x, tmp.z);
		float norm = dir - angle.y;

		// 角度があってなかったら
		if (abs(norm) > 0.0f) {
			// 歩きのアニメーションを流す
			PlayWalkAnim();

			// 自身との向きを-π～+πの範囲にする
			while (norm > DX_PI_F) {

				norm -= DX_TWO_PI_F;
			}
			while (norm < -DX_PI_F) {

				norm += DX_TWO_PI_F;
			}

			// 右なら足す
			if (norm > 0.0f) {
				if (norm > Deg2Rad(2.0f)) {
					angle.y += Deg2Rad(2.0f);
				}
				else {
					angle.y = dir;
				}
			}
			// 左なら引く
			else {
				if (norm < -Deg2Rad(2.0f)) {
					angle.y -= Deg2Rad(2.0f);
				}
				else {
					angle.y = dir;
				}
			}
		}
		else {

			// 角度合わせがないならアイドル
			PlayIdleAnim();
		}
		return;
	}

	float distance = (*playerPos.at(target) - pos).Length();
	int luck = GetRand(10000);

	if (luck <= 4000) {
		if (distance <= 450.0f) {
			KickDownChangeState();
		}
		else {
			MoveChangeState();
		}
	}
	else if (luck <= 8000) {

		StraightChangeState();
	}
	else {
		StampChangeState();
	}
}

void GrapeBossIdleState::Exit(void)
{
}

void GrapeBossIdleState::AlwaysUpdate(void)
{
}
