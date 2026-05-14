#pragma once
#include <DxLib.h>
#include <vector>
#include <algorithm> 

#include "../UI_Base.h"

#include "../../../Application/Application.h"

#include "../../../Common/Vector2.h"
#include "../../Character/CharacterStatsDefine.h"

class PlayerHpUI : public UI_Base
{
private:

	enum class IMAGE_KINDS
	{
		FRAME,
		HP_GAUGE,
		DAMAGE_GAUGE,

		MAX
	};

#pragma region 定数定義
	const Vector2I HP_IMAGE_SIZE = Vector2I(354.0f, 50.0f);		// HPバーの画像サイズ

	const Vector2I HP_UI_POS = Vector2I(0, App::SCREEN_SIZE_Y_HALF);	// 画像の座標
	const int HP_UI_POS_OFFSET = 10;	// フレーム画像とHPバー画像の微妙なズレのオフセット

	const short PLAYER_HP_MAX;	// HPの最大値
#pragma endregion 定数定義

public:

	PlayerHpUI(const CharacterStats& stats);	// コンストラクタ
	~PlayerHpUI() override = default;	//デストラクタ

private:

	void Load(void) override;
	void SubInit(void) override {}	// 初期化
	void SubUpdate(void) override;		// 更新
	void SubDraw(void) override;		// 描画
	void SubRelease(void) override;		// 解放


#pragma region 変数定義
	const short& playerhp;	// プレイヤーのHP

	float hpRatio;	// HP割合
	float hpBarOffset;	// HPの割合に応じてHP描画を調整するためのオフセット

	float damageBarOffset;
#pragma endregion 変数定義

};