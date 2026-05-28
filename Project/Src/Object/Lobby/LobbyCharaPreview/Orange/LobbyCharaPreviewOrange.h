#pragma once

#include "../LobbyCharaPreviewBase.h"

class LobbyCharaPreviewOrange : public LobbyCharaPreviewBase
{
public:
	LobbyCharaPreviewOrange(const Vector3& pos) : LobbyCharaPreviewBase(pos) {}
	~LobbyCharaPreviewOrange()override = default;;

	void Load(void)override {
		// モデルを読み込む
		trans.Load("Charactor/Orange/OrangeModel");

		// ライティングの調整
		MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

		// モデルのスケール
		trans.scale = 2.5f;

		// アニメーションを登録する
		AddAnimation("Data/Model/Charactor/CommonAnime/Dance.mv1", 1);
	}
};