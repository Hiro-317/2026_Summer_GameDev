#include "BossSelectSkyShipStartState.h"

#include <algorithm>

#include "../../../../Manager/Camera/CurrentCamera.h"
#include "../../../../Manager/Net/NetWorkManager.h"

BossSelectSkyShipStartState::BossSelectSkyShipStartState(
	const Vector3& LOCAL_START_POS,

	int POS_MAX_FRAME,
	int ANGLE_MAX_FRAME,

	Vector3& pos,
	Quaternion& rotation,

	const std::function<void(void)> MoveChangeState
) :
	LOCAL_START_POS(LOCAL_START_POS),

	POS_MAX_FRAME(POS_MAX_FRAME),
	ANGLE_MAX_FRAME(ANGLE_MAX_FRAME),

	START_POS(), START_ROTATION(),
	END_POS(), END_ROTATION(),

	pos(pos),
	rotation(rotation),

	MoveChangeState(MoveChangeState),

	currentFrame(0),

	state(STATE::None)
{
}

void BossSelectSkyShipStartState::Enter(void)
{
	END_POS = pos;
	END_ROTATION = rotation;

	pos += LOCAL_START_POS;

	Vector3 vec = (END_POS - pos).Normalized();
	rotation = Quaternion::FromEulerXZY(Vector3(-asinf(std::clamp(vec.y, -1.0f, 1.0f)), atan2f(vec.x, vec.z), 0.0f)).Normalized();

	START_POS = pos;
	START_ROTATION = rotation;

	state = STATE::PosMove;
}

void BossSelectSkyShipStartState::Update(void)
{
	currentFrame++;

	switch (state) {

	case BossSelectSkyShipStartState::STATE::PosMove: {

		if (currentFrame >= POS_MAX_FRAME) {
			currentFrame = 0;
			pos = END_POS;
			state = STATE::AngleMove;
			break;
		}

		float rate = static_cast<float>(currentFrame) / static_cast<float>(POS_MAX_FRAME);

		rate = std::clamp(rate, 0.0f, 1.0f);

		rate = rate * rate * (3.0f - 2.0f * rate);

		pos = START_POS + (END_POS - START_POS) * rate;

		break;
	}

	case BossSelectSkyShipStartState::STATE::AngleMove: {

		if (currentFrame >= ANGLE_MAX_FRAME) {
			currentFrame = 0;
			rotation = END_ROTATION;
			state = STATE::None;
			MoveChangeState();
			break;
		}

		float rate = static_cast<float>(currentFrame) / static_cast<float>(ANGLE_MAX_FRAME);

		rate = std::clamp(rate, 0.0f, 1.0f);

		rate = rate * rate * (3.0f - 2.0f * rate);

		rotation = Quaternion::Slerp(START_ROTATION, END_ROTATION, rate).Normalized();

		break;
	}

	}
}

void BossSelectSkyShipStartState::Exit(void)
{
	pos = END_POS;
	rotation = END_ROTATION;
	CurrentCamera::Get().EndEvent();
	Net::GetIns().Send(MsgDataCameraEvent(MsgDataCameraEvent::TYPE::EndEvent));
}