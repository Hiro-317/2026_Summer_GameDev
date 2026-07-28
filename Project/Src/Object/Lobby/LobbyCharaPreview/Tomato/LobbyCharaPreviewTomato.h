#pragma once

#include "../LobbyCharaPreviewBase.h"

class LobbyCharaPreviewTomato : public LobbyCharaPreviewBase
{
public:
	LobbyCharaPreviewTomato(const Vector3& pos, unsigned char operatorNumber) : LobbyCharaPreviewBase(pos, operatorNumber) {}
	~LobbyCharaPreviewTomato()override = default;;

private:

	void CharacterLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Tomato/Tomato");

		// モデルのスケール
		trans.scale = 2.5f;
	}
};