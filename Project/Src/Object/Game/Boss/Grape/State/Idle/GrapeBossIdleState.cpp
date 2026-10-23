#include "GrapeBossIdleState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../Common/Transform/Transform.h"

GrapeBossIdleState::GrapeBossIdleState(
	Transform& bossTrans,
	
	const Vector3* const(&playerPos)[(int)MSG_SENDER_ID::Max],
	
	const int& targetNum,
	const float& coolTime,
	
	const std::function<void(void)>& playIdleAnim,
	const std::function<void(void)>& playWalkAnim,
	const std::function<void(void)>& moveChangeState,
	const std::function<void(void)>& kickDownChangeState,
	const std::function<void(void)>& straightChangeState,
	const std::function<void(void)>& stampChangeState,
	const std::function<void(void)>& singleChangeState,
	const std::function<void(void)>& stalkerChangeState,
	const std::function<void(void)>& randomChangeState
) :
	bossTrans(bossTrans),

	playerPos(playerPos),

	targetNum(targetNum),

	coolTime(coolTime),

	playIdleAnim(playIdleAnim),
	playWalkAnim(playWalkAnim),

	moveChangeState(moveChangeState),
	kickDownChangeState(kickDownChangeState),
	straightChangeState(straightChangeState),
	stampChangeState(stampChangeState),
	singleChangeState(singleChangeState),
	stalkerChangeState(stalkerChangeState),
	randomChangeState(randomChangeState),

	angleFlg()
{
}

void GrapeBossIdleState::Enter(void)
{
	playIdleAnim();

	angleFlg = true;
}

void GrapeBossIdleState::Update(void)
{
	// クールタイムの間
	if (coolTime > 0.0f) {

		// プレイヤーとの角度を出す
		Vector3 tmp = *playerPos[targetNum] - bossTrans.pos;
		float dir = atan2f(tmp.x, tmp.z);
		Vector3 forward = bossTrans.rotation.Rotate(Vector3(0.0f, 0.0f, 1.0f));
		float norm = dir - atan2f(forward.x, forward.z);

		// 角度があってなかったらフラグを立てる
		if (abs(norm) > NOT_ROT) { angleFlg = true; }

		// 角度合わせフラグが立ってるなら
		if (angleFlg) {
			// 歩きのアニメーションを流す
			playWalkAnim();

			// 自身との向きを-π～+πの範囲にする
			while (norm > DX_PI_F) { norm -= DX_TWO_PI_F; }
			while (norm < -DX_PI_F) { norm += DX_TWO_PI_F; }

			// 右なら足す
			if (norm > 0.0f) {
				if (norm > ROT_POW) {
					bossTrans.rotation = Quaternion::FromRotationY(atan2f(forward.x, forward.z) + ROT_POW);
				}
				else {
					// 角度があってるならおろす
					bossTrans.rotation = Quaternion::FromRotationY(dir);
					angleFlg = false;
				}
			}
			// 左なら引く
			else {
				if (norm < -ROT_POW) {
					bossTrans.rotation = Quaternion::FromRotationY(atan2f(forward.x, forward.z) - ROT_POW);
				}
				else {
					// 角度があってるならおろす
					bossTrans.rotation = Quaternion::FromRotationY(dir);
					angleFlg = false;
				}
			}
		}
		else {

			// 角度合わせがないならアイドル
			playIdleAnim();

		}
		return;
	}

	// 距離と確率を出す
	float distance = (*playerPos[targetNum] - bossTrans.pos).Length();
	int luck = GetRand(RANDOM);

	// 確率で変える
	if (luck <= KICKDOWN_LUCK) {
		// 距離が近いなら頭突き
		if (distance <= DISTANCE) {
			kickDownChangeState();
		}
		// 遠いなら移動
		else {
			moveChangeState();
		}
	}
	else if (luck <= STRAIGHT_LUCK) {
		// 距離が遠いなら直線投擲
		if (distance >= DISTANCE) {
			straightChangeState();
		}
		// 近いならかかと落としか追従
		else if (luck <= RELOOT_LUCK) {
			kickDownChangeState();
		}
		else {
			stalkerChangeState();
		}
	}
	else if (luck <= STAMP_LUCK) {
		stampChangeState();
	}
	else if (luck <= SINGLE_LUCK) {
		singleChangeState();
	}
	else if (luck <= STALKER_LUCK) {
		stalkerChangeState();
	}
	else {
		randomChangeState();
	}
}

void GrapeBossIdleState::Exit(void)
{
}

void GrapeBossIdleState::AlwaysUpdate(void)
{
}
