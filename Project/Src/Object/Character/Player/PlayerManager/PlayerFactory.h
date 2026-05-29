#pragma once
#include "../Orange/OrangePlayer.h"
#include "../../../CharaTypeDefine.h"

#include "../../../../Scene/SceneManager/SceneManager.h"

struct PlayerInfo
{
	PlayerBase* instance;
};

class PlayerFactory
{
public:

	static PlayerInfo CreatePlayer(MSG_SENDER_ID id) {
		PlayerInfo ret = {};
		CHARA_TYPE chara = SceneManager::GetIns().GetSelectCharaType(id);

		switch (chara) {
		case CHARA_TYPE::Orange: { ret.instance = new OrangePlayer(id); break; }
		case CHARA_TYPE::Tomato: { ret.instance = new OrangePlayer(id); break; }
		default: { ret.instance = nullptr; break; }
		}

		return ret;
	}

};