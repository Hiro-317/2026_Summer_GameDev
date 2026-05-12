#include "PlayerUI.h"

PlayerUI::PlayerUI(Vector2I pos, const int& coolTimeCounter, int COOL_TIME) :
	coolTimeCounter(coolTimeCounter),
	coolTimeRatio(0.0f),
	offset(0.0f),
	COOL_TIME(COOL_TIME),
	pos(pos)
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
	//coolTimeCounter--;
	//if (coolTimeCounter < 0) {
	//	coolTimeCounter = 0;
	//}
}

void PlayerUI::Draw(void)
{
	coolTimeRatio = (float)coolTimeCounter / (float)COOL_TIME;
	offset = IMAGE_SIZE * (1.0f - coolTimeRatio);

	int offset_I = (int)offset;
	
	DrawGraph(
		pos.x - (IMAGE_SIZE / 2),
		pos.y - (IMAGE_SIZE / 2),
		images.at(0),
		true
	);
	
	DrawRectGraph(
		pos.x - (IMAGE_SIZE / 2),
		(pos.y + IMAGE_SIZE / 2) - offset_I,
		0, IMAGE_SIZE - offset_I,
		IMAGE_SIZE, offset_I,
		images.at(1),
		true,
		false
	);
}


