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

		// ライティングの調整
		MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

		// モデルのスケール
		trans.scale = 2.5f;
	}
};