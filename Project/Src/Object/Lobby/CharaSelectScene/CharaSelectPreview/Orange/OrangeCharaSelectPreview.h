#pragma once

#include "../CharaSelectPreviewBase.h"

class OrangeCharaSelectPreview : public CharaSelectPreviewBase
{
public:
	OrangeCharaSelectPreview() :
		CharaSelectPreviewBase("Data/Parameter/Character/Player/Orange/")
	{
	}
	~OrangeCharaSelectPreview()override = default;

private:

	void SubLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Orange/OrangeModel");

		trans.scale = 2.0f;
		trans.centerDiff = Vector3::Yonly(-265);
		trans.angle.x = Deg2Rad(15.0f);

		// アニメーションを登録する
		AddAnimation("Data/Model/Character/Orange/Anime/Idle.mv1", 1.0f);

		CHARA_NAME = "リモネン";

		HP = GetParameterToInt("OrangeParameter", "PlayerHP");
		ATTACK = GetParameterToInt("OrangeParameter", "PlayerAttackPower");
		DEFENCE = GetParameterToInt("OrangeParameter", "PlayerDefensePower");
		SPEED = GetParameterToInt("OrangeParameter", "PlayerMoveSpeed");

		skill_1_image = LoadGraph("Data/Image/Lobby/CharaSelect/Orange/Skill1.png");
		skill_2_image = LoadGraph("Data/Image/Lobby/CharaSelect/Orange/Skill2.png");
		skill_3_image = LoadGraph("Data/Image/Lobby/CharaSelect/Orange/Skill3.png");
	}
};