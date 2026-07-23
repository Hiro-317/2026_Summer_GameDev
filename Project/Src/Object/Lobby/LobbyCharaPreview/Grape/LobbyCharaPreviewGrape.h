#pragma once

#include "../LobbyCharaPreviewBase.h"

class LobbyCharaPreviewGrape : public LobbyCharaPreviewBase
{
public:
	LobbyCharaPreviewGrape(const Vector3& pos, unsigned char operatorNumber) : LobbyCharaPreviewBase(pos, operatorNumber) {}
	~LobbyCharaPreviewGrape()override = default;;

private:

	void CharacterLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Grape/GrapeModel");

		// ライティングの調整
		MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

		// モデルのスケール
		trans.scale = 2.5f;

		AddInFbxAnimation(11, 1.0f, 0);
	}
};