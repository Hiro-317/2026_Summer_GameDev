#pragma once

#include "../CharaSelectPreviewBase.h"

class TomatoCharaSelectPreview : public CharaSelectPreviewBase
{
public:
	TomatoCharaSelectPreview() :
		CharaSelectPreviewBase()
	{
	}
	~TomatoCharaSelectPreview()override = default;

private:

	void SubLoad(void)override {
		// モデルを読み込む
		trans.Load("Charactor/Tomato/Tomato");
		// ライティングの調整
		MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

		trans.scale = 2;
		trans.centerDiff = Vector3::Yonly(-250);

		CHARA_NAME = "リコピン";

		HP = 0;
		ATTACK = 0;
		DEFENCE = 0;
		SPEED = 0;

		skill_1_image = LoadGraph("Data/Image/Lobby/CharaSelect/Tomato/Skill1.png");
		skill_2_image = LoadGraph("Data/Image/Lobby/CharaSelect/Tomato/Skill2.png");
		skill_3_image = LoadGraph("Data/Image/Lobby/CharaSelect/Tomato/Skill3.png");
	}
};