#pragma once

#include "../LobbyCharaPreviewBase.h"

class LobbyCharaPreviewOrange : public LobbyCharaPreviewBase
{
public:
	LobbyCharaPreviewOrange(const Vector3& pos, unsigned char operatorNumber) : LobbyCharaPreviewBase(pos, operatorNumber) {}
	~LobbyCharaPreviewOrange()override = default;;

private:

	void CharacterLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Orange/OrangeModel");

		// モデルのスケール
		trans.scale = 2.5f;

		// アニメーションを登録する
		AddAnimation("Data/Model/Character/CommonAnime/Dance.mv1", 1);
	}
};