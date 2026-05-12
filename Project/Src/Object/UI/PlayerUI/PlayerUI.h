#pragma once

#include <DxLib.h>
#include <vector>
#include <map>
#include <string>

#include "../../../Common/Vector2.h"

class PlayerUI
{
private:

	const Vector2I IMAGE_SIZE = Vector2I(150, 258 / 2);
public:

	enum class UI_TYPE
	{
		SKILL,
		HP,
	};

	enum class SKILL_UI_COLOR
	{
		GREEN,

		MAX
	};

	PlayerUI(Vector2I pos, const int& coolTimeCounter, int COOL_TIME, SKILL_UI_COLOR color);
	~PlayerUI();

	void Update(void);
	void Draw(void);
	
	const bool GetIsCoolTimeNow(int STATE_TAG) { return (coolTimeCounter > 0); }

private:

	// 描画位置
	Vector2I pos;

	const int& coolTimeCounter;	// クールタイムカウンター用変数
	float coolTimeRatio;		// クールタイム割合変数
	float offset;				// 描画するときのずれを修正するためのオフセット変数

	const int COOL_TIME;

	std::map<SKILL_UI_COLOR, std::string> chargingImagePath =
	{
		{ SKILL_UI_COLOR::GREEN, "Data/Image/UI/Player/SkillSlotGreenCharging.png" }
	};
	std::map<SKILL_UI_COLOR, std::string> chargeImagePath =
	{
		{ SKILL_UI_COLOR::GREEN, "Data/Image/UI/Player/SkillSlotGreenCharge.png" }
	};

	std::vector<int> images;	// 画像を読み込むためのハンドル
};

