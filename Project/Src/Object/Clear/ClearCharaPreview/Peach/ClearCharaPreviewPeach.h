#pragma once

#include "../ClearCharaPreviewBase.h"

class ClearCharaPreviewPeach : public ClearCharaPreviewBase
{
public:
	ClearCharaPreviewPeach(const Vector3& pos, unsigned char operatorNumber) : ClearCharaPreviewBase(pos, operatorNumber) {}
	~ClearCharaPreviewPeach()override = default;;

	void Load(void)override {
		// モデルを読み込む
		trans.LoadModel("Character/Peach/PeachModel");

		// モデルのスケール
		trans.scale = 2.5f;

		AddInFbxAnimation(7, 1.0f, true, 0);
	}

private:

	void SubInit(void)override {

		// ライティングの調整
		MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	}
};