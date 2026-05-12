#include "PlayerUI.h"

PlayerUI::PlayerUI(Vector2I pos, const int& coolTimeCounter, int COOL_TIME, SKILL_UI_COLOR color) :
	coolTimeCounter(coolTimeCounter),
	coolTimeRatio(0.0f),
	offset(0.0f),
	COOL_TIME(COOL_TIME),
	pos(pos)
{
	// 画像の読み込み
	images.reserve(10);
	
		images.push_back(LoadGraph("Data/Image/UI/Player/SkillSlotFrame.png"));
		images.push_back(LoadGraph(chargingImagePath.find(color)->second.c_str()));
		images.push_back(LoadGraph(chargeImagePath.find(color)->second.c_str()));
}

PlayerUI::~PlayerUI()
{
	// 画像の解放
	for (int& image : images) {
		DeleteGraph(image);
	}
}

void PlayerUI::Update()
{
}

void PlayerUI::Draw(void)
{
	// スキルUIのフレーム
	DrawGraph(
		pos.x - (IMAGE_SIZE.x / 2),
		pos.y - (IMAGE_SIZE.y / 2),
		images.at(0),
		true
	);

	// クールタイムの量に応じて増減する
	coolTimeRatio = (float)coolTimeCounter / (float)COOL_TIME;
	offset = IMAGE_SIZE.y * (1.0f - coolTimeRatio);

	int offset_I = (int)offset;
	
	// スキルのUI
	DrawRectGraph(
		pos.x - (IMAGE_SIZE.x / 2),
		(pos.y + IMAGE_SIZE.y / 2) - offset_I,
		0, IMAGE_SIZE.y - offset_I,
		IMAGE_SIZE.x, offset_I,
		images.at(1),
		true,
		false
	);

	if (coolTimeRatio <= 0.0f) {
		DrawGraph(
			pos.x - (IMAGE_SIZE.x / 2),
			pos.y - (IMAGE_SIZE.y / 2),
			images.at(2),
			true
		);
	}
}


