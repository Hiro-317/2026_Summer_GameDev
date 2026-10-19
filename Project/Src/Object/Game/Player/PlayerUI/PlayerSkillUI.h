#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

#include <map>

#include "../../../../Common/Vector2I.h"


class PlayerSkillUI : public ActorBase
{
public:

	// UIの色
	enum class SKILL_UI_COLOR
	{
		Green,
		Blue,
		Red,
		Purple,
		Yellow,
		Orange,
		Indigo,

		Max
	};

	/// <summary>
	/// スキルUI生成
	/// </summary>
	/// <param name="pos">描画位置（中心座標）</param>
	/// <param name="COOL_TIME">クールタイムの最大値</param>
	/// <param name="coolTimeCounter">クールタイム参照</param>
	/// <param name="color">色</param>
	/// <param name="skillImagePath">スキル画像の名前</param>
	/// <param name="isChargeSkill">チャージ攻撃かどうか</param>
	PlayerSkillUI(
		const Vector2I& pos,
		int COOL_TIME,
		const float& coolTimeCounter,
		SKILL_UI_COLOR color,
		const std::string& skillImageName,
		bool isChargeSkill = false
	);

	~PlayerSkillUI() = default;	// デストラクタ

	void Load(void)override {}
	void Update(void)override {}
	
private:

	// 画像をタイプ別に分ける
	enum class IMAGE_TYPE
	{
		Frame,

		Skill,
		ColorImage1,
		ColorImage2,
		ChargeOk,

		MAX
	};

	// 階層
	const std::string IMAGE_FILE_PATH = "Data/Image/Game/UI/Player/Skill/";

	// 画像の名前（チャージ中背景）
	static const inline std::map<SKILL_UI_COLOR, std::string> CHARGING_IMAGE_NAME_TABLE =
	{
		{ SKILL_UI_COLOR::Green,	"SkillSlotGreenCharging"	},
		{ SKILL_UI_COLOR::Blue,		"SkillSlotBlueCharging"		},
		{ SKILL_UI_COLOR::Red,		"SkillSlotRedCharging"		},
		{ SKILL_UI_COLOR::Purple,	"SkillSlotPurpleCharging"	},
		{ SKILL_UI_COLOR::Yellow,	"SkillSlotYellowCharging"	},
		{ SKILL_UI_COLOR::Orange,	"SkillSlotOrangeCharging"	},
		{ SKILL_UI_COLOR::Indigo,	"SkillSlotIndigoCharging"	}
	};

	// 画像の名前（チャージ完了背景）
	static const inline std::map<SKILL_UI_COLOR, std::string> CHARGE_IMAGE_NAME_TABLE =
	{
		{ SKILL_UI_COLOR::Green,	"SkillSlotGreenCharge"	 },
		{ SKILL_UI_COLOR::Blue,		"SkillSlotBlueCharge"	 },
		{ SKILL_UI_COLOR::Red,		"SkillSlotRedCharge"	 },
		{ SKILL_UI_COLOR::Purple,	"SkillSlotPurpleCharge"	 },
		{ SKILL_UI_COLOR::Yellow,	"SkillSlotYellowCharge"	 },
		{ SKILL_UI_COLOR::Orange,	"SkillSlotOrangeCharge"	 },
		{ SKILL_UI_COLOR::Indigo,	"SkillSlotIndigoCharge"	 }
	};

	// スキルの画像サイズ
	const Vector2I IMAGE_SIZE = Vector2I(150, 129);

	// 画像ハンドル（枠）
	int frameImage;
	// 画像ハンドル（チャージ中背景）
	int chargingBackImage;
	// 画像ハンドル（チャージ完了背景）
	int chargeCompleteBackImage;
	// 画像ハンドル（スキル）
	int skillImage;
	// 画像ハンドル（チャージ可能）（特殊）
	int reloadableImage;

	// 描画位置
	Vector2I pos;

	// クールタイムの最大値
	const int COOL_TIME;
	// クールタイムの参照
	const float& coolTimeCounter;

	// チャージスキルかどうかのフラグ
	const bool isChargeSkill;

	// 初期化処理
	void SubInit(void)override;

	// 描画処理
	void SubDraw(void)override;

	// 解放処理
	void SubRelease(void)override;
};

