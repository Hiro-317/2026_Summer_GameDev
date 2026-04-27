#include "PlayerUI.h"

PlayerUI::PlayerUI(Vector2 pos, int COOL_TIME) :
	images(),
	pos(pos),
	COOL_TIME(COOL_TIME),
	coolTimeCounter(0.0f),
	coolTimeRatio(0.0f),
	offset(0.0f)
{
	// 画像の読み込み
	images.reserve(10);
	images.push_back(LoadGraph("Data/Image/UI/Player/Skill_Image_1.png"));
	images.push_back(LoadGraph("Data/Image/UI/Player/Skill_Image_2.png"));
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
	coolTimeCounter--;
	if (coolTimeCounter < 0) {
		coolTimeCounter = 0;
	}
}

void PlayerUI::Draw(void)
{
	coolTimeRatio = (float)coolTimeCounter / (float)COOL_TIME;
	offset = IMAGE_SIZE * (1.0f - coolTimeRatio);

	DrawRectGraph(
		pos.x - (IMAGE_SIZE / 2), pos.y - (IMAGE_SIZE / 2),
		0, 0,
		IMAGE_SIZE, IMAGE_SIZE,
		images.at(0),
		true,
		false
	);

	DrawRectGraph(
		pos.x - (IMAGE_SIZE / 2),
		(pos.y + IMAGE_SIZE / 2) - offset,
		0,
		IMAGE_SIZE - offset,
		IMAGE_SIZE,
		offset,
		images.at(1),
		true,
		false
	);
}
