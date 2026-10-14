#pragma once

#include "../../../../Manager/Net/SenderID_Define.h"

class PlayerBase;

struct PlayerInfo
{
	PlayerBase* instance;
};

class PlayerFactory
{
public:

	// ID番目のプレイヤーが選んだキャラを生成し、returnする
	static PlayerInfo CreatePlayer(MSG_SENDER_ID id);

};