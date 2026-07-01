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

};