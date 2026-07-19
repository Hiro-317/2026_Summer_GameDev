#pragma once

#include "../CharaSelectPreviewBase.h"

class TomatoCharaSelectPreview : public CharaSelectPreviewBase
{
public:
	TomatoCharaSelectPreview() :
		CharaSelectPreviewBase("Data/Parameter/Character/Player/Tomato/")
	{
	}
	~TomatoCharaSelectPreview()override = default;

private:

	void SubLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Tomato/Tomato");
		// ライティングの調整
		MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

		trans.scale = 2;
		trans.centerDiff = Vector3::Yonly(-250);

		CHARA_NAME = "リコピン";


		HP = GetParameterToInt("TomatoParameter", "PlayerHP");
		ATTACK = GetParameterToInt("TomatoParameter", "PlayerAttackPower");
		DEFENCE = GetParameterToInt("TomatoParameter", "PlayerDefensePower");
		SPEED = GetParameterToInt("TomatoParameter", "PlayerMoveSpeed");

		skill_1_image = LoadGraph("Data/Image/Lobby/CharaSelect/Tomato/Skill1.png");
		skill_2_image = LoadGraph("Data/Image/Lobby/CharaSelect/Tomato/Skill2.png");
		skill_3_image = LoadGraph("Data/Image/Lobby/CharaSelect/Tomato/Skill3.png");
	}
};