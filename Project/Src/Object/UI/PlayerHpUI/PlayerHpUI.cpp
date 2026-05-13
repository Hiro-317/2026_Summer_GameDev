#include "PlayerHpUI.h"


PlayerHpUI::PlayerHpUI(
	const short& player1hp, const short PLAYER1_HP_MAX, 
	const short& player2hp, const short PLAYER2_HP_MAX, 
	const short& player3hp, const short PLAYER3_HP_MAX,
	const short& player4hp, const short PLAYER4_HP_MAX
) :
	player1hp(player1hp), PLAYER1_HP_MAX(PLAYER1_HP_MAX),
	player2hp(player2hp), PLAYER2_HP_MAX(PLAYER2_HP_MAX),
	player3hp(player3hp), PLAYER3_HP_MAX(PLAYER3_HP_MAX),
	player4hp(player4hp), PLAYER4_HP_MAX(PLAYER4_HP_MAX),
	hpImages(),
	hpFrameImage(-1)
{
}

PlayerHpUI::~PlayerHpUI()
{
}

void PlayerHpUI::Update()
{

}

void PlayerHpUI::Draw()
{

}

void PlayerHpUI::Release()
{
	for (int i = 0; i < HP_IMAGE_MAX; i++) {
		DeleteGraph(hpImages[i]);
	}

	DeleteGraph(hpFrameImage);
}