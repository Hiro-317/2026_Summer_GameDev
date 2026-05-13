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
	images[(int)IMAGE_TYPE::FRAME] = LoadGraph("Data/Image/UI/Player/Skill/SkillSlotFrame.png");
	images[(int)IMAGE_TYPE::SKILL] = LoadGraph(("Data/Image/UI/Player/Skill/" + skillImagePath + ".png").c_str());
	images[(int)IMAGE_TYPE::COLOR_IMAGE1] = LoadGraph(chargeImagePath.find(color)->second.c_str());
	images[(int)IMAGE_TYPE::COLOR_IMAGE2] = LoadGraph(chargingImagePath.find(color)->second.c_str());
}



PlayerSkillUI::~PlayerSkillUI()
{

}

void PlayerSkillUI::Update()
{
	// クールタイムの量に応じて増減する
	coolTimeRatio = (float)coolTimeCounter / (float)COOL_TIME;
	offset = IMAGE_SIZE.y * (1.0f - coolTimeRatio);
}

void PlayerSkillUI::Draw(void)
{
	// スキルUIのフレーム
	DrawGraph(
		pos.x - (IMAGE_SIZE.x / 2),
		pos.y - (IMAGE_SIZE.y / 2),
		images[(int)IMAGE_TYPE::FRAME],
		true
	);

	int offset_I = (int)offset;
	
	// スキルのUI
	DrawRectGraph(
		pos.x - (IMAGE_SIZE.x / 2),
		(pos.y + IMAGE_SIZE.y / 2) - offset_I,
		0, IMAGE_SIZE.y - offset_I,
		IMAGE_SIZE.x, offset_I,
		images[(int)IMAGE_TYPE::COLOR_IMAGE1],
		true,
		false
	);

	// クールタイムがないときこの画像を描画する
	if (coolTimeRatio <= 0.0f) {
		DrawRotaGraph(
			pos.x,
			pos.y,
			1.0f,
			0.0f,
			images[(int)IMAGE_TYPE::COLOR_IMAGE2],
			true
		);
	}

	// スキルによって変わる画像の画像
	DrawRotaGraph(
		pos.x,
		pos.y,
		1.0f,
		0.0f,
		images[(int)IMAGE_TYPE::SKILL],
		true
	);
}

void PlayerSkillUI::Release(void)
{ 
	// 画像の解放
	for (int& image : images) {
		DeleteGraph(image);
	}
}
