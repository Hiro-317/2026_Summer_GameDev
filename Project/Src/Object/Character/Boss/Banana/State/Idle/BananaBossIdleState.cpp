#include "BananaBossIdleState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

BananaBossIdleState::BananaBossIdleState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	Vector3& pos, Vector3& angle,
	const std::vector<const Vector3*> playerPos,
	const std::function<int(void)> CoolTime,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayIdleAnim/*,
	const std::function<void(void)> KickDownChangeState,
	const std::function<void(void)> StraightChangeState,
	const std::function<void(void)> StampChangeState,
	const std::function<void(void)> SingleChangeState,
	const std::function<void(void)> StalkerChangeState,
	const std::function<void(void)> RandomChangeState*/
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), angle(angle),
	playerPos(playerPos),
	CoolTime(CoolTime),
	GetTarget(GetTarget),
	PlayIdleAnim(PlayIdleAnim),
	/*KickDownChangeState(KickDownChangeState),
	StraightChangeState(StraightChangeState),
	StampChangeState(StampChangeState),
	SingleChangeState(SingleChangeState),
	StalkerChangeState(StalkerChangeState),
	RandomChangeState(RandomChangeState),*/
	cnt(0), target(0), angleFlg(true)
{
}

void BananaBossIdleState::Enter(void)
{
	cnt = CoolTime();
	target = GetTarget();
	PlayIdleAnim();
	angleFlg = true;
}

void BananaBossIdleState::Update(void)
{
	// クールタイムの間
	if (cnt > 0) {
		cnt--;
		
		return;
	}
	// 距離と確率を出す
	float distance = (*playerPos.at(target) - pos).Length();
	int luck = GetRand(RANDOM);

	//// 確率で変える
	//if (luck <= KICKDOWN_LUCK) {
	//	// 距離が近いなら頭突き
	//	if (distance <= DISTANCE) {
	//		KickDownChangeState();
	//	}
	//}
	//else if (luck <= STRAIGHT_LUCK) {
	//	// 距離が遠いなら直線投擲
	//	if (distance >= DISTANCE) {
	//		StraightChangeState();
	//	}
	//	// 近いならかかと落としか追従
	//	else if (luck <= RELOOT_LUCK){
	//		KickDownChangeState();
	//	}
	//	else {
	//		StalkerChangeState();
	//	}
	//}
	//else if (luck <= STAMP_LUCK) {

	//	StampChangeState();
	//}
	//else if (luck <= SINGLE_LUCK) {

	//	SingleChangeState();
	//}
	//else if (luck <= STALKER_LUCK) {

	//	StalkerChangeState();
	//}
	//else {
	//	RandomChangeState();
	//}
}

void BananaBossIdleState::Exit(void)
{
}

void BananaBossIdleState::AlwaysUpdate(void)
{
}
