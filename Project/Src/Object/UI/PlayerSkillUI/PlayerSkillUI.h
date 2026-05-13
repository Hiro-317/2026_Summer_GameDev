#pragma once

#include <DxLib.h>
#include <vector>
#include <map>
#include <string>

#include "../../../Common/Vector2.h"


class PlayerSkillUI
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
		BLUE,
		RED,

		MAX
	};

	/// <summary>
	/// スキルUI生成
	/// </summary>
	/// <param name="pos">描画位置</param>
	/// <param name="coolTimeCounter">スキルのクールタイムの変数</param>
	/// <param name="COOL_TIME">クールタイムの最大値</param>
	/// <param name="color">UIの色</param>
	/// <param name="skillImagePath">スキルの画像パス</param>
	/// <param name="imageSizeRate">スキルの画像サイズ倍率</param>
	PlayerSkillUI(
		Vector2I pos, 
		const int& coolTimeCounter, 
		int COOL_TIME, 
		SKILL_UI_COLOR color,
		std::string skillImagePath
	);

	// デストラクタ
	~PlayerSkillUI();

	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理
	
	// クールタイムのゲッター関数
	const bool GetIsCoolTimeNow(int STATE_TAG) { return (coolTimeCounter > 0); }

private:

#pragma region スキルUI関係

	std::map<SKILL_UI_COLOR, std::string> chargingImagePath =
	{
		{ SKILL_UI_COLOR::GREEN, "Data/Image/UI/Player/SkillSlotGreenCharging.png" },
		{ SKILL_UI_COLOR::BLUE, "Data/Image/UI/Player/SkillSlotBlueCharging.png" },
		{ SKILL_UI_COLOR::RED, "Data/Image/UI/Player/SkillSlotRedCharging.png" }
	};
	std::map<SKILL_UI_COLOR, std::string> chargeImagePath =
	{
		{ SKILL_UI_COLOR::GREEN, "Data/Image/UI/Player/SkillSlotGreenCharge.png" },
		{ SKILL_UI_COLOR::BLUE, "Data/Image/UI/Player/SkillSlotBlueCharge.png" },
		{ SKILL_UI_COLOR::RED, "Data/Image/UI/Player/SkillSlotRedCharge.png" }
	};

	// 描画位置
	Vector2I pos = Vector2I(0);

	float coolTimeRatio = 0.0f;		// クールタイム割合変数
	float offset = 0.0f;				// 描画するときのずれを修正するためのオフセット変数

	const int COOL_TIME;		// クールタイムの最大値
	const int& coolTimeCounter;	// クールタイムカウンター用変数

#pragma endregion
	std::vector<int> images;	// 画像を読み込むためのハンドル
};

