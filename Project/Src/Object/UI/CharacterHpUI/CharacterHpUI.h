#pragma once

#include "../../../pch.h"

#include <vector>
#include <algorithm> 

#include "../UI_Base.h"

#include "../../../Application/Application.h"

#include "../../../Common/Vector2.h"
#include "../../Character/CharacterStatsDefine.h"

class CharacterHpUI : public UI_Base
{
public:
	enum class CHARACTER_KINDS
	{
		PLAYER,
		BOSS,

		MAX
	};

	CharacterHpUI(const CharacterStats& stats, const CHARACTER_KINDS characterKinds);	// コンストラクタ
	~CharacterHpUI() override = default;	//デストラクタ

private:

	void Load(void) override;
	void SubInit(void) override {}	// 初期化
	void SubUpdate(void) override;		// 更新
	void SubDraw(void) override;		// 描画
	void SubRelease(void) override;		// 解放

	enum class IMAGE_KINDS
	{
		FRAME,
		HP_GAUGE,
		DAMAGE_GAUGE,

		MAX
	};

#pragma region 定数定義
	// 画像の名前
	const char* HP_FRAME_IMAGE_NAME[(int)CHARACTER_KINDS::MAX] = {
		"PlayerHpFrame",
		"BossHpFrame",
	};
	const char* HP_IMAGE_NAME[(int)CHARACTER_KINDS::MAX] = {
		"PlayerHp",
		"BossHp",
	};
	const char* HP_LOST_IMAGE_NAME[(int)CHARACTER_KINDS::MAX] = {
		"PlayerHpLost",
		"BossHpLost",
	};
	// 画像のファイルパスタイプ
	FILE_PATH_TYPE HP_IMAGE_FILE_PATH_TYPE[(int)CHARACTER_KINDS::MAX] = {
		FILE_PATH_TYPE::PLAYER_HP,
		FILE_PATH_TYPE::BOSS_HP,
	};

	// HPバーの画像サイズ
	const Vector2I HP_IMAGE_SIZE[(int)CHARACTER_KINDS::MAX] = {
		Vector2I(354, 50),
		Vector2I(629, 50),
	};

	const short PLAYER_HP_MAX;	// HPの最大値

	const float DAMAGE_GAUGE_DEC = 0.5f;	// ダメージを受けた時の赤いゲージの減少量

	const short HP_GAUGE_OFFSET[(int)CHARACTER_KINDS::MAX] = {
		10,
		18,
	};

	const Vector2I DEFAULT_UI_POS[(int)CHARACTER_KINDS::MAX] = {
		Vector2I(0, App::SCREEN_SIZE_Y_HALF),
		Vector2I(App::SCREEN_SIZE_X_HALF-314,0),
	};

	const CHARACTER_KINDS CHARA_KINDS;
#pragma endregion 定数定義

#pragma region 変数定義
	const short& playerhp;	// プレイヤーのHP

	float hpRatio;	// HP割合
	float hpBarOffset;	// HPの割合に応じてHP描画を調整するためのオフセット

	float damageBarOffset;

	float damageBarAxcel;
#pragma endregion 変数定義

};