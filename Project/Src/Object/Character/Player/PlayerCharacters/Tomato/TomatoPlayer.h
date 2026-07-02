#pragma once

#include "../../PlayerBase.h"

#include "../../../../../Common/Vector2.h"

class TomatoPlayer : public PlayerBase
{
public:

	TomatoPlayer(MSG_SENDER_ID operatorSenderId);
	~TomatoPlayer()override = default;

	void PlayerLoad(void)override;

	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;

private:

#pragma region ƒXƒLƒ‹‚P‚Ì’è”’è‹`

	const int SKILL1_COOL_TIME = GetParameter("Skill1", "CoolTime");

	const float MOVE_SPEED = 60.0f;

	const float ROTATION_POW = Deg2Rad(40.0f);

#pragma endregion 

};