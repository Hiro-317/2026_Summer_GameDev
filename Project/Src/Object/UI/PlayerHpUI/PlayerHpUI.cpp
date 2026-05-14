#include "PlayerHpUI.h"


PlayerHpUI::PlayerHpUI(const CharacterStats& stats) :
	playerhp(stats.hp),
	PLAYER_HP_MAX(stats.hpMax.Value()),
	hpRatio(0.0f),
	hpBarOffset(0.0f),
	damageBarOffset(0.0f)
{
}

void PlayerHpUI::Load(void)
{
	LoadUIImage("PlayerHpFrame", (int)IMAGE_KINDS::FRAME,			FILE_PATH_TYPE::HP);
	LoadUIImage("PlayerHp",		 (int)IMAGE_KINDS::HP_GAUGE,		FILE_PATH_TYPE::HP);
	LoadUIImage("PlayerHpLost",  (int)IMAGE_KINDS::DAMAGE_GAUGE,	FILE_PATH_TYPE::HP);
}

void PlayerHpUI::SubUpdate()
{
	// HPの割合によるHPバーの増減のための計算
	hpRatio = (float)playerhp / (float)PLAYER_HP_MAX;
	hpBarOffset = HP_IMAGE_SIZE.x * (1.0f - hpRatio);

	if (damageBarOffset < hpBarOffset) {
		damageBarOffset += 0.1f;
		if (damageBarOffset > hpBarOffset) {
			damageBarOffset = hpBarOffset;
		}
	}
}

void PlayerHpUI::SubDraw()
{
	// HPのフレーム画像描画
	DrawGraph(HP_UI_POS.x, HP_UI_POS.y, uiImages.at((int)IMAGE_KINDS::FRAME), true);

	// ダメージを受けたときの赤いゲージ
	DrawRectGraph(
		HP_UI_POS.x + HP_UI_POS_OFFSET,
		HP_UI_POS.y,
		0, 0,
		HP_IMAGE_SIZE.x - damageBarOffset,
		HP_IMAGE_SIZE.y,
		uiImages.at((int)IMAGE_KINDS::DAMAGE_GAUGE),
		true
	);

	// HPバーの描画
	DrawRectGraph(
		HP_UI_POS.x + HP_UI_POS_OFFSET,
		HP_UI_POS.y,
		0, 0,
		HP_IMAGE_SIZE.x - hpBarOffset, HP_IMAGE_SIZE.y,
		uiImages.at((int)IMAGE_KINDS::HP_GAUGE),
		true
	);
}

void PlayerHpUI::SubRelease()
{
}