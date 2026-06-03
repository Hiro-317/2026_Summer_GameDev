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

	// コンストラクタ
	CharacterHpUI(
		const CharacterStats& stats,

		const char* HP_FRAME_IMAGE_NAME,
		const char* HP_IMAGE_NAME,
		const char* HP_LOST_IMAGE_NAME,

		const Vector2I& HP_IMAGE_SIZE,
		short HP_GAUGE_OFFSET,

		const Vector2I& HP_UI_POS,

		const std::string CHARA_NAME
	);	

	//デストラクタ
	~CharacterHpUI() override = default;	

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
	const char* HP_FRAME_IMAGE_NAME;
	const char* HP_IMAGE_NAME;
	const char* HP_LOST_IMAGE_NAME;

	// HPバーの画像サイズ
	const Vector2I HP_IMAGE_SIZE;

	const short HP_GAUGE_OFFSET = 10;

	const Vector2I HP_UI_POS = Vector2I(0, App::SCREEN_SIZE_Y_HALF);

	const std::string CHARA_NAME;		// HPの上に描画する名前

	const short HP_MAX;

#pragma endregion 定数定義

#pragma region 変数定義
	const short& playerhp;	// プレイヤーのHP

	float hpRatio;	// HP割合
	float hpBarOffset;	// HPの割合に応じてHP描画を調整するためのオフセット

	float damageBarOffset;	// ダメージを受けた時に描画されるバー
	float damageBarAxcel;

#pragma endregion 変数定義

};