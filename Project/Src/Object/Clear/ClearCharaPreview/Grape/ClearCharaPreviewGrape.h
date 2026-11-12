#pragma once

#include "../ClearCharaPreviewBase.h"

class ClearCharaPreviewGrape : public ClearCharaPreviewBase
{
public:
	ClearCharaPreviewGrape(const Vector3& pos, unsigned char operatorNumber) : ClearCharaPreviewBase(pos, operatorNumber) {}
	~ClearCharaPreviewGrape()override = default;;

	void Load(void)override {
		// モデルを読み込む
		trans.LoadModel("Character/Grape/GrapeModel");

		// モデルのスケール
		trans.scale = 2.5f;
		
		AddInFbxAnimation(11, 1.0f, true, 0);
	}
};