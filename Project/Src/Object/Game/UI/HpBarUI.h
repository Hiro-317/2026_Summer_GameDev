#pragma once

#include "../../Common/ActorBase/ActorBase.h"

#include "../../../Common/Vector2I.h"

struct CharacterStats;

class HpBarUI : public ActorBase
{
public:
	/// <summary>
	/// HPバー（アイコン付き）
	/// </summary>
	/// <param name="HP_FRAME_IMAGE_PATH">画像パス（HP枠）</param>
	/// <param name="HP_BAR_IMAGE_PATH">画像パス（HPバー緑）</param>
	/// <param name="HP_LOST_BAR_IMAGE_PATH">画像パス（HPバー赤）</param>
	/// <param name="HP_GAUGE_OFFSET">HPフレーム画像サイズ</param>
	/// <param name="HP_BAR_IMAGE_SIZE">HPバー画像サイズ</param>
	/// <param name="UI_POS">描画位置</param>
	/// <param name="stats">ステータス</param>
	/// <param name="ICON_BACK_IMAGE_PATH">画像パス（アイコン背景）</param>
	/// <param name="ICON_CHARA_IMAGE_PATH">画像パス（アイコンキャラ）</param>
	/// <param name="ICON_FRAME_IMAGE_PATH">画像パス（アイコン枠）</param>
	/// <param name="ICON_IMAGE_SIZE">アイコン画像サイズ</param>
	HpBarUI(
		const std::string& HP_FRAME_IMAGE_PATH,
		const std::string& HP_BAR_IMAGE_PATH,
		const std::string& HP_LOST_BAR_IMAGE_PATH,

		const Vector2I& HP_FRAME_IMAGE_SIZE,
		const Vector2I& HP_BAR_IMAGE_SIZE,
		const Vector2I& UI_POS,

		const CharacterStats& stats,

		const std::string& ICON_BACK_IMAGE_PATH = "",
		const std::string& ICON_CHARA_IMAGE_PATH = "",
		const std::string& ICON_FRAME_IMAGE_PATH = "",

		const Vector2I& ICON_IMAGE_SIZE = Vector2I()
	);
	~HpBarUI()override = default;

	void Load(void)override;

	void Init(void)override;

	void Update(void)override;

private:

#pragma region 定数

	// HPフレーム画像サイズ
	const Vector2I HP_FRAME_IMAGE_SIZE;
	// HPバー画像サイズ
	const Vector2I HP_BAR_IMAGE_SIZE;

	// 描画位置
	const Vector2I UI_POS;

	// アイコン画像サイズ
	const Vector2I ICON_IMAGE_SIZE;

	// HPが減った時のダメージバーの毎フレーム減少する量
	const float HP_DAMAGE_BAR_DEC = 0.5;

#pragma endregion

	// ステータスの参照
	const CharacterStats& stats;

	// 画像ハンドル（HP枠）
	int hpFrameImage;
	// 画像ハンドル（HPバー緑）
	int hpBarImage;
	// 画像ハンドル（HPバー赤）
	int hpLostBarImage;

	// 画像ハンドル（アイコン背景）
	int iconBackImage;
	// 画像ハンドル（アイコンキャラ）
	int iconCharaImage;
	// 画像ハンドル（アイコン枠）
	int iconFrameImage;

	// HP割合（HPバー緑）（リアルタイムのHP割合）
	float drawHpBarRatio;
	// HP割合（HPバー赤）（少し補間したHP割合）
	float drawHpLostBarRatio;

	// 描画
	void SubDraw(void)override;

	// 解放
	void SubRelease(void)override;
};