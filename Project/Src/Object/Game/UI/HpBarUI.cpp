#include "HpBarUI.h"

#include "../../../Utility/Utility.h"

#include "../../../Manager/TimeScale/TimeScale.h"

#include "../../Common/CharacterBase/CharacterStatsDefine.h"

HpBarUI::HpBarUI(
	const std::string& HP_FRAME_IMAGE_PATH,
	const std::string& HP_BAR_IMAGE_PATH,
	const std::string& HP_LOST_BAR_IMAGE_PATH,

	const Vector2I& HP_FRAME_IMAGE_SIZE,
	const Vector2I& HP_BAR_IMAGE_SIZE,
	const Vector2I& UI_POS,

	const CharacterStats& stats,

	const std::string& ICON_BACK_IMAGE_PATH,
	const std::string& ICON_CHARA_IMAGE_PATH,
	const std::string& ICON_FRAME_IMAGE_PATH,

	const Vector2I& ICON_IMAGE_SIZE
) :
	ActorBase(),

	hpFrameImage(LoadGraph(HP_FRAME_IMAGE_PATH.c_str())),
	hpBarImage(LoadGraph(HP_BAR_IMAGE_PATH.c_str())),
	hpLostBarImage(LoadGraph(HP_LOST_BAR_IMAGE_PATH.c_str())),

	HP_FRAME_IMAGE_SIZE(HP_FRAME_IMAGE_SIZE),
	HP_BAR_IMAGE_SIZE(HP_BAR_IMAGE_SIZE),
	UI_POS(UI_POS),

	stats(stats),

	iconBackImage(ICON_BACK_IMAGE_PATH == "" ? -1 : LoadGraph(ICON_BACK_IMAGE_PATH.c_str())),
	iconCharaImage(ICON_CHARA_IMAGE_PATH == "" ? -1 : LoadGraph(ICON_CHARA_IMAGE_PATH.c_str())),
	iconFrameImage(ICON_FRAME_IMAGE_PATH == "" ? -1 : LoadGraph(ICON_FRAME_IMAGE_PATH.c_str())),

	ICON_IMAGE_SIZE(ICON_IMAGE_SIZE),

	drawHpBarRatio(0.0f),
	drawHpLostBarRatio(0.0f)
{
}

void HpBarUI::Load(void)
{
}

void HpBarUI::Init(void)
{
	// UI描画に設定する
	SetDrawType(ACTOR_DRAW_TYPE::Ui);

	// HP割合を初期化
	drawHpBarRatio = drawHpLostBarRatio = 1.0f;
}

void HpBarUI::Update(void)
{
	// HP割合（HPバー緑）（リアルタイムのHP割合）
	drawHpBarRatio = (float)stats.hp / (float)stats.hpMax.Value();

	// HP割合（HPバー赤）（少し補間したHP割合）
	drawHpLostBarRatio = Lerp(drawHpLostBarRatio, drawHpBarRatio, 0.25f * TimeScale::Get());
}

void HpBarUI::SubDraw(void)
{
	// 描画位置
	Vector2I drawPos = UI_POS;

	// 読み込まれていたらアイコンから描画する（その場合はアイコンサイズ分HPバーがズレることになる）
	if (iconBackImage != -1 && iconCharaImage != -1 && iconFrameImage != -1) {

		// 描画（アイコン背景）
		DrawRotaGraph(drawPos.x, drawPos.y, 1, 0, iconBackImage, true);
		// 描画（アイコンキャラ）
		DrawRotaGraph(drawPos.x, drawPos.y, 1, 0, iconCharaImage, true);
		// 描画（アイコン枠）
		DrawRotaGraph(drawPos.x, drawPos.y, 1, 0, iconFrameImage, true);
		
		// 描画が終わったらズラし、アイコンの右端を描画開始位置にする
		drawPos.x += ICON_IMAGE_SIZE.x / 2;
	}

	// バーの増減を含めた描画の為に、描画位置を左上に合わせる
	drawPos.y -= HP_FRAME_IMAGE_SIZE.y / 2;

	// 描画（HP枠）
	DrawGraph(drawPos.x, drawPos.y, hpFrameImage, true);

	// 描画位置をオフセット分ズラす
	drawPos += (HP_FRAME_IMAGE_SIZE - HP_BAR_IMAGE_SIZE) / 2;

	// 描画（HPバー赤）
	DrawRectGraph(
		drawPos.x,
		drawPos.y,
		0, 0,
		(int)((float)HP_BAR_IMAGE_SIZE.x * drawHpLostBarRatio),
		HP_BAR_IMAGE_SIZE.y,
		hpLostBarImage,
		true
	);

	// 描画（HPバー緑）
	DrawRectGraph(
		drawPos.x,
		drawPos.y,
		0, 0,
		(int)((float)HP_BAR_IMAGE_SIZE.x * drawHpBarRatio),
		HP_BAR_IMAGE_SIZE.y,
		hpBarImage,
		true
	);
}

void HpBarUI::SubRelease(void)
{
	// 画像の解放（アイコン枠）
	if (iconFrameImage != -1) {
		DeleteGraph(iconFrameImage);
		iconFrameImage=-1;
	}
	// 画像の解放（アイコンキャラ）
	if (iconCharaImage != -1) {
		DeleteGraph(iconCharaImage);
		iconCharaImage = -1;
	}
	// 画像の解放（アイコン背景）
	if (iconBackImage != -1) {
		DeleteGraph(iconBackImage);
		iconBackImage = -1;
	}

	// 画像の解放（HPバー赤）
	if (hpLostBarImage != -1) {
		DeleteGraph(hpLostBarImage);
		hpLostBarImage = -1;
	}
	// 画像の解放（HPバー緑）
	if (hpBarImage != -1) {
		DeleteGraph(hpBarImage);
		hpBarImage = -1;
	}
	// 画像の解放（HP枠）
	if (hpFrameImage != -1) {
		DeleteGraph(hpFrameImage);
		hpFrameImage = -1;
	}
}
