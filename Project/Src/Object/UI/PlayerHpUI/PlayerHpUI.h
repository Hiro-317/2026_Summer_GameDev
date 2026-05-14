#pragma once
#include <vector>

#include "../../../Application/Application.h"

#include "../../../Common/Vector2.h"
#include "../../Character/CharacterStatsDefine.h"

class PlayerHpUI
{
private:

#pragma region 定数定義
	const Vector2I HP_IMAGE_SIZE = Vector2I(354.0f, 50.0f);		// HPバーの画像サイズ

	const Vector2I HP_UI_POS = Vector2I(0, App::SCREEN_SIZE_Y_HALF);	// 画像の座標
	const int HP_UI_POS_OFFSET = 10;	// フレーム画像とHPバー画像の微妙なズレのオフセット

	const short PLAYER_HP_MAX;	// HPの最大値
#pragma endregion 定数定義

public:

	PlayerHpUI(const CharacterStats& stats);	// コンストラクタ
	~PlayerHpUI();	//デストラクタ

	void Load(void);		// 読み込み
	void Init(void);
	void Update(void);		// 更新
	void Draw(void);		// 描画
	void Release(void);		// 解放

private:

#pragma region 変数定義
	const short& playerhp;	// プレイヤーのHP

	float hpRatio;	// HP割合
	float offset;	// HPの割合に応じてHP描画を調整するためのオフセット

	std::vector<int> hpImages;	// HPの画像

	int hpFrameImage;	// HPの枠
#pragma endregion 変数定義

};