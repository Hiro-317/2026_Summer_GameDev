#include "PlayerUI.h"

PlayerUI::PlayerUI(Vector2 pos) :
	ui_info()
{
	ui_info.pos = pos;

	// 画像の読み込み
	ui_info.images.reserve(10);
	ui_info.images.push_back(LoadGraph("Data/Image/UI/Player/Skill_Image_1.png"));
	ui_info.images.push_back(LoadGraph("Data/Image/UI/Player/Skill_Image_2.png"));
}

PlayerUI::~PlayerUI()
{
	// 画像の解放
	for (int& image : ui_info.images) {
		DeleteGraph(image);
	}
}

void PlayerUI::Update()
{
	ui_info.coolTimeCounter--;
	if (ui_info.coolTimeCounter < 0) {
		ui_info.coolTimeCounter = 0;
	}
}

void PlayerUI::Draw(void)
{
	ui_info.coolTimeRatio = (float)ui_info.coolTimeCounter / (float)ui_info.COOL_TIME;
	ui_info.offset = IMAGE_SIZE * (1.0f - ui_info.coolTimeRatio);
	
	DrawGraph(
		ui_info.pos.x - (IMAGE_SIZE / 2),
		ui_info.pos.y - (IMAGE_SIZE / 2),
		ui_info.images.at(0),
		true
	);
	
	DrawRectGraph(
		ui_info.pos.x - (IMAGE_SIZE / 2),
		(ui_info.pos.y + IMAGE_SIZE / 2) - ui_info.offset,
		0, IMAGE_SIZE - ui_info.offset,
		IMAGE_SIZE, ui_info.offset,
		ui_info.images.at(1),
		true,
		false
	);
}

