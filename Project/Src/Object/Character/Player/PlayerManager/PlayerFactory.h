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

	// プレイヤーを生成
	static PlayerInfo CreatePlayer(MSG_SENDER_ID id) {
		PlayerInfo ret = {};
		// プレイヤー選択画面で選択されたキャラを取得
		CHARA_TYPE chara = SceneManager::GetIns().GetSelectCharaType(id);

		switch (chara) {
		case CHARA_TYPE::Orange: { ret.instance = new OrangePlayer(id); break; }	// オレンジ
		//case CHARA_TYPE::Tomato: { ret.instance = new TomatoPlayer(id); break; }	// トマト
		default: { ret.instance = nullptr; break; }
		}

		return ret;
	}

};