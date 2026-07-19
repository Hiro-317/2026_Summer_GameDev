#pragma once

#include "../CharaSelectPreviewBase.h"

class PeachCharaSelectPreview : public CharaSelectPreviewBase
{
public:
	PeachCharaSelectPreview() :
		CharaSelectPreviewBase("Data/Parameter/Character/Player/Peach/")
	{
	}
	~PeachCharaSelectPreview()override = default;

private:

	void SubLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Peach/PeachModel");
		// ライティングの調整
		MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

		trans.scale = 2;
		trans.centerDiff = Vector3::Yonly(-250);

		// アニメーションを登録する
		AddInFbxAnimation(7, 1.0f, 0);

		CHARA_NAME = "おももこまち";

		HP = GetParameterToInt("PeachParameter", "PlayerHP");
		ATTACK = GetParameterToInt("PeachParameter", "PlayerAttackPower");
		DEFENCE = GetParameterToInt("PeachParameter", "PlayerDefensePower");
		SPEED = GetParameterToInt("PeachParameter", "PlayerMoveSpeed");

		skill_1_image = LoadGraph("Data/Image/Lobby/CharaSelect/Orange/Skill1.png");
		skill_2_image = LoadGraph("Data/Image/Lobby/CharaSelect/Orange/Skill2.png");
		skill_3_image = LoadGraph("Data/Image/Lobby/CharaSelect/Orange/Skill3.png");
	}
};