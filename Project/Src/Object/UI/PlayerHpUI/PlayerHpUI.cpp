#include "PlayerHpUI.h"



PlayerHpUI::PlayerHpUI(const CharacterStats& stats) :
	playerhp(stats.hp),
	PLAYER_HP_MAX(stats.hpMax.Value()),
	hpRatio(0.0f),
	offset(0.0f),
	hpImages(),
	hpFrameImage(-1)
{
}

PlayerHpUI::~PlayerHpUI()
{
}

void PlayerHpUI::Load(void)
{
	hpImages.reserve(2);
	// 画像の読み込み
	hpImages.emplace_back(LoadGraph("Data/Image/UI/Player/HP/PlayerHpFrame.png"));
	hpImages.emplace_back(LoadGraph("Data/Image/UI/Player/HP/PlayerHp.png"));
}

void PlayerHpUI::Update()
{
	// HPの割合によるHPバーの増減のための計算
	hpRatio = (float)playerhp / (float)PLAYER_HP_MAX;
	offset = HP_IMAGE_SIZE.x * (1.0f - hpRatio);
}

void PlayerHpUI::Draw()
{
	// HPのフレーム画像描画
	DrawGraph(HP_UI_POS.x, HP_UI_POS.y, hpImages.at(0), true);

	// HPバーの描画
	DrawRectGraph(
		HP_UI_POS.x + HP_UI_POS_OFFSET, 
		HP_UI_POS.y,
		0, 0,
		HP_IMAGE_SIZE.x - offset, HP_IMAGE_SIZE.y,
		hpImages.at(1),
		true
	);
}

void PlayerHpUI::Release()
{
	// 画像の解放
	for (int& image : hpImages) {
		DeleteGraph(image);
	}

	DeleteGraph(hpFrameImage);
}