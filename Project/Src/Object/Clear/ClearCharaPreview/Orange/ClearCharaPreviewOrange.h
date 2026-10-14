#pragma once

#include "../ClearCharaPreviewBase.h"

class ClearCharaPreviewOrange : public ClearCharaPreviewBase
{
public:
	ClearCharaPreviewOrange(const Vector3& pos, unsigned char operatorNumber) : ClearCharaPreviewBase(pos, operatorNumber) {}
	~ClearCharaPreviewOrange()override = default;;

	void Load(void)override {
		// モデルを読み込む
		trans.LoadModel("Character/Orange/OrangeModel");

		// モデルのスケール
		trans.scale = 1.5f;

		// モデルのズレ
		trans.centerDiff = Vector3(0, 60, 0) * trans.scale;

		// アニメーションを登録する
		AddInFbxAnimation(10, 1.0f, true, 0);
	}
};