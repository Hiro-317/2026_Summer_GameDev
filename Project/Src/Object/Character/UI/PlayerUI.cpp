#include "PlayerUI.h"

PlayerUI::PlayerUI()
{
	images.reserve(10);
	images.push_back(LoadGraph("Data/Image/UI/Player/Skill_Image_1.png"));
	images.push_back(LoadGraph("Data/Image/UI/Player/Skill_Image_2.png"));
}

PlayerUI::~PlayerUI()
{
	for (int& image : images) { DeleteGraph(image); }
}

void PlayerUI::Draw(void)
{
	for (int& image : images) {
		DrawRectGraph(
			0, 0,
			0, SIZE,
			SIZE, SIZE,
			image,
			true, false
		);
	}

}