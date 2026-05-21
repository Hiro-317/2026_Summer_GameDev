#pragma once

#include "../../../pch.h"
#include <vector>
#include <map>
#include <string>

#include "../UI_Base.h"

#include "../../../Common/Vector2.h"


class PlayerSkillUI : public UI_Base
{
private:
	const Vector2I IMAGE_SIZE = Vector2I(150, 129);
public:

	// UIの色
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


	~PlayerSkillUI();	// デストラクタ

	void Load(void)override {}
	void SubInit(void) override {}
	void SubUpdate(void) override;	// 更新処理
	void SubDraw(void) override;	// 描画処理
	void SubRelease(void) override;	// 解放処理
	
	// クールタイムのゲッター関数
	const bool GetIsCoolTimeNow(int STATE_TAG) { return (coolTimeCounter > 0); }

private:
	// 画像をタイプ別に分ける
	enum class IMAGE_TYPE
	{
		FRAME,
		SKILL,
		COLOR_IMAGE1,
		COLOR_IMAGE2,

		MAX
	};

	// チャージ中の色別に分けた画像
	std::map<SKILL_UI_COLOR, std::string> chargingImagePath =
	{
		{ SKILL_UI_COLOR::GREEN, "SkillSlotGreenCharging" },
		{ SKILL_UI_COLOR::BLUE, "SkillSlotBlueCharging" },
		{ SKILL_UI_COLOR::RED, "SkillSlotRedCharging" }
	};

	// チャージしていない時の色別の画像
	std::map<SKILL_UI_COLOR, std::string> chargeImagePath =
	{
		{ SKILL_UI_COLOR::GREEN, "SkillSlotGreenCharge" },
		{ SKILL_UI_COLOR::BLUE, "SkillSlotBlueCharge" },
		{ SKILL_UI_COLOR::RED, "SkillSlotRedCharge" }
	};


	Vector2I pos;		// 描画位置

	float coolTimeRatio;		// クールタイム割合変数
	float offset;				// 描画するときのずれを修正するためのオフセット変数

	const int COOL_TIME;		// クールタイムの最大値
	const int& coolTimeCounter;	// クールタイムカウンター用変数
};

