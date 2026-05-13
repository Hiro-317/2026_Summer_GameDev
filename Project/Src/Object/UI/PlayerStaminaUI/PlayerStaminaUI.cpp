#include "PlayerStaminaUI.h"

PlayerStaminaUI::PlayerStaminaUI(const short& stamina, const short STAMINA_MAX):
	images(),
	stamina(stamina),
	STAMINA_MAX(STAMINA_MAX)
{
}

PlayerStaminaUI::~PlayerStaminaUI()
{
}

void PlayerStaminaUI::Load()
{
	LoadDivGraph(
		"Data/Image/UI/Player/Stamina/PlayerStamina.png",
		IMAGE_MAX,
		IMAGE_X_NUM, IMAGE_Y_NUM,
		IMAGE_X_SIZE, IMAGE_Y_SIZE,
		images
	);

	pos = DEFAULT_POS;
}

void PlayerStaminaUI::Draw()
{
	// スタミナの割合で配列を動かす
	int index = 0;
	if (GetStaminaIndex(stamina, STAMINA_MAX, index)) {
		// スタミナの描画
		DrawGraph(pos.x, pos.y, images[index], true);
	}
}

void PlayerStaminaUI::Release()
{
	for (int i = 0; i < IMAGE_MAX; i++) {
		DeleteGraph(images[i]);	// 画像の解放
	}
}