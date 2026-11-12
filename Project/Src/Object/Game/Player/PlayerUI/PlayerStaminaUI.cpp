#include "PlayerStaminaUI.h"

#include "../../../../pch.h"

#include "../../../../Common/Vector2.h"

#include "../../../../Manager/Camera/CurrentCamera.h"

PlayerStaminaUI::PlayerStaminaUI(short STAMINA_MAX, const float& stamina, const Vector3& playerPos) :
	images(),

	STAMINA_MAX(STAMINA_MAX),
	stamina(stamina),

	playerPos(playerPos)
{
}

void PlayerStaminaUI::Load()
{
	// 画像のロード
	LoadDivGraph(
		"Data/Image/Game/UI/Player/Stamina/PlayerStamina.png",
		IMAGE_MAX,
		IMAGE_X_NUM, IMAGE_Y_NUM,
		IMAGE_X_SIZE, IMAGE_Y_SIZE,
		images
	);
}

void PlayerStaminaUI::SubInit()
{
	// UI描画に設定する
	SetDrawType(ACTOR_DRAW_TYPE::Ui);
}

void PlayerStaminaUI::SubDraw()
{
	// スタミナの割合で配列を動かす
	int index = 0;
	if (GetStaminaIndex(stamina, STAMINA_MAX, index)) {

		CurrentCamera::Get().Apply();

		Vector3 temp = ConvWorldPosToScreenPos(playerPos.ToVECTOR());
		Vector2 drawPos = Vector2(temp.x, temp.y);

		// スタミナの描画
		DrawRotaGraph(drawPos.x + 100, drawPos.y - 50, 1, 0, images[index], true);
	}
}

void PlayerStaminaUI::SubRelease()
{
	// 画像の解放
	for (int i = 0; i < IMAGE_MAX; i++) {
		DeleteGraph(images[i]);	// 画像の解放
		images[i] = -1;
	}
}