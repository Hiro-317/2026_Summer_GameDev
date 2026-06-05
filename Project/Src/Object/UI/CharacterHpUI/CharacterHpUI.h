#pragma once

#include "../../../pch.h"

#include <vector>
#include <algorithm> 
#include <string>

#include "../UI_Base.h"

#include "../../../Application/Application.h"

#include "../../../Common/Vector2.h"
#include "../../Character/CharacterStatsDefine.h"

class CharacterHpUI : public UI_Base
{
public:

	// コンストラクタ
	CharacterHpUI(
		const CharacterStats& stats,

		const std::string HP_FRAME_IMAGE_NAME,
		const std::string HP_IMAGE_NAME,
		const std::string HP_LOST_IMAGE_NAME,

		const Vector2I& HP_IMAGE_SIZE,
		short HP_GAUGE_OFFSET,
		const Vector2I& HP_UI_POS,

		const FILE_PATH_TYPE PATH_TYPE,
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

	short const GetHp(void) { return playerhp; }

#pragma region 定数定義

	// 画像の名前
	const std::string HP_FRAME_IMAGE_NAME;	// フレームの画像パス
	const std::string HP_IMAGE_NAME;		// HPバーの画像パス
	const std::string HP_LOST_IMAGE_NAME;	// ダメージを受けた時のHPバーの画像パス

	// HPバーの画像サイズ
	const Vector2I HP_IMAGE_SIZE;	// HP画像サイズ
	const short HP_GAUGE_OFFSET;	// HP画像とフレーム画像のズレ
	const Vector2I HP_UI_POS;		// HP画像の描画位置

	const std::string CHARA_NAME;		// HPの上に描画する名前

	const FILE_PATH_TYPE PATH_TYPE;		// なんのファイルパスか

	const short HP_MAX;

	const float HP_DAMAGE_BAR_DEC = 0.5;

#pragma endregion 定数定義

#pragma region 変数定義
	const short& playerhp;	// プレイヤーのHP

	float hpRatio;	// HP割合
	float hpBarOffset;	// HPの割合に応じてHP描画を調整するためのオフセット

	float damageBarOffset;	// ダメージを受けた時に描画されるバー
	float damageBarAxcel;

#pragma endregion 変数定義

};