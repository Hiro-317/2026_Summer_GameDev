#include "PlayerSkillUI.h"

PlayerSkillUI::PlayerSkillUI(
	Vector2I pos, 
	const int& coolTimeCounter, 
	int COOL_TIME, 
	SKILL_UI_COLOR color,
	std::string skillImagePath
) :
	pos(pos),
	COOL_TIME(COOL_TIME),
	coolTimeRatio(0.0f),
	offset(0.0f),
	coolTimeCounter(coolTimeCounter)
{
	// 画像の読み込み
	UILoadImage("SkillSlotFrame", (int)IMAGE_TYPE::FRAME, FILE_PATH_TYPE::PLAYER_SKILL);
	UILoadImage(skillImagePath, (int)IMAGE_TYPE::SKILL, FILE_PATH_TYPE::PLAYER_SKILL);
	UILoadImage(chargeImagePath.find(color)->second.c_str(), (int)IMAGE_TYPE::COLOR_IMAGE1, FILE_PATH_TYPE::PLAYER_SKILL);
	UILoadImage(chargingImagePath.find(color)->second.c_str(), (int)IMAGE_TYPE::COLOR_IMAGE2, FILE_PATH_TYPE::PLAYER_SKILL);
}



PlayerSkillUI::~PlayerSkillUI()
{

}

void PlayerSkillUI::SubUpdate()
{
	// クールタイムの量に応じて増減する
	coolTimeRatio = (float)coolTimeCounter / (float)COOL_TIME;
	offset = IMAGE_SIZE.y * (1.0f - coolTimeRatio);
}

void PlayerSkillUI::SubDraw(void)
{
	// スキルUIのフレーム
	DrawGraph(
		pos.x - (IMAGE_SIZE.x / 2),
		pos.y - (IMAGE_SIZE.y / 2),
		uiImages.at((int)IMAGE_TYPE::FRAME),
		true
	);

	int offset_I = (int)offset;
	
	// クールタイムが動いているときこの画像を描画する
	DrawRectGraph(
		pos.x - (IMAGE_SIZE.x / 2),
		(pos.y + IMAGE_SIZE.y / 2) - offset_I,
		0, IMAGE_SIZE.y - offset_I,
		IMAGE_SIZE.x, offset_I,
		uiImages.at((int)IMAGE_TYPE::COLOR_IMAGE1),
		true
	);

	// クールタイムがないときこの画像を描画する
	if (coolTimeRatio <= 0.0f) {
		DrawRotaGraph(
			pos.x,
			pos.y,
			1.0f,
			0.0f,
			uiImages.at((int)IMAGE_TYPE::COLOR_IMAGE2),
			true
		);
	}

	// スキルによって変わる画像の画像
	DrawRotaGraph(
		pos.x,
		pos.y,
		1.0f,
		0.0f,
		uiImages.at((int)IMAGE_TYPE::SKILL),
		true
	);
}

void PlayerSkillUI::SubRelease(void)
{ 
}
