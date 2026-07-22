#include "CharacterHpUI.h"

#include "../../../Manager/Font/FontManager.h"


CharacterHpUI::CharacterHpUI(
	const short& hp,
	const short HP_MAX,

	const std::string HP_FRAME_IMAGE_NAME,
	const std::string HP_IMAGE_NAME,
	const std::string HP_LOST_IMAGE_NAME,
	const std::string ICON_CHARA_IMAGE_NAME,
	const std::string ICON_FRAME_IMAGE_NAME,
	const std::string ICON_SELECT_IMAGE_NAME,

	const Vector2I HP_IMAGE_SIZE,
	const Vector2I HP_GAUGE_OFFSET,
	const Vector2I HP_UI_POS,

	const FILE_PATH_TYPE PATH_TYPE,
	const std::string CHARA_NAME
) :
	hp(hp),
	HP_MAX(HP_MAX),

	HP_FRAME_IMAGE_NAME(HP_FRAME_IMAGE_NAME),
	HP_IMAGE_NAME(HP_IMAGE_NAME),
	HP_LOST_IMAGE_NAME(HP_LOST_IMAGE_NAME),
	ICON_CHARA_IMAGE_NAME(ICON_CHARA_IMAGE_NAME),
	ICON_FRAME_IMAGE_NAME(ICON_FRAME_IMAGE_NAME),
	ICON_SELECT_IMAGE_NAME(ICON_SELECT_IMAGE_NAME),

	HP_IMAGE_SIZE(HP_IMAGE_SIZE),
	HP_GAUGE_OFFSET(HP_GAUGE_OFFSET),
	HP_UI_POS(HP_UI_POS),

	PATH_TYPE(PATH_TYPE),
	CHARA_NAME(CHARA_NAME),

	hpRatio(0.0f),
	hpBarOffset(0.0f),
	damageBarOffset(0.0f),
	damageBarAxcel(0.0f)
{
}

void CharacterHpUI::Load(void)
{
	// 画像のロード
	UILoadImage(HP_FRAME_IMAGE_NAME,(int)IMAGE_KINDS::FRAME,		PATH_TYPE);
	UILoadImage(HP_IMAGE_NAME,		(int)IMAGE_KINDS::HP_GAUGE,		PATH_TYPE);
	UILoadImage(HP_LOST_IMAGE_NAME,	(int)IMAGE_KINDS::DAMAGE_GAUGE, PATH_TYPE);

	// プレイヤーならロードする
	if (PATH_TYPE == FILE_PATH_TYPE::PLAYER_HP) {
		UILoadImage(ICON_CHARA_IMAGE_NAME,	(int)IMAGE_KINDS::ICON_CHARA,	PATH_TYPE);
		UILoadImage(ICON_FRAME_IMAGE_NAME,	(int)IMAGE_KINDS::ICON_FRAME,	PATH_TYPE);
		UILoadImage(ICON_SELECT_IMAGE_NAME, (int)IMAGE_KINDS::ICON_BACK,	PATH_TYPE);
	}
}

void CharacterHpUI::SubUpdate()
{
	// HPの割合によるHPバーの増減のための計算
	hpRatio = (float)hp / (float)HP_MAX;
	hpBarOffset = HP_IMAGE_SIZE.x * (1.0f - hpRatio);

	if (damageBarOffset < hpBarOffset) {
		// ダメージを受けたので赤いゲージを後から減少
		damageBarOffset += HP_DAMAGE_BAR_DEC;
		// HPを上回って減らないようにする
		if (damageBarOffset > hpBarOffset) {
			damageBarOffset = hpBarOffset;
		}
	}
}

void CharacterHpUI::SubDraw()
{
	Vector2I uiPos = HP_UI_POS;

	// HPのフレーム画像描画
	DrawGraph(uiPos.x, uiPos.y, uiImages.at((int)IMAGE_KINDS::FRAME), true);

	// ダメージを受けたときの赤いゲージ
	DrawRectGraph(
		uiPos.x + HP_GAUGE_OFFSET.x,
		uiPos.y,
		0, 0,
		HP_IMAGE_SIZE.x - (int)damageBarOffset,
		HP_IMAGE_SIZE.y,
		uiImages.at((int)IMAGE_KINDS::DAMAGE_GAUGE),
		true
	);

	// HPの緑のゲージの描画
	DrawRectGraph(
		uiPos.x + HP_GAUGE_OFFSET.x,
		uiPos.y,
		0, 0,
		HP_IMAGE_SIZE.x - (int)hpBarOffset, 
		HP_IMAGE_SIZE.y,
		uiImages.at((int)IMAGE_KINDS::HP_GAUGE),
		true
	);

	if (PATH_TYPE != FILE_PATH_TYPE::PLAYER_HP) { return; }

	DrawRotaGraph(
		uiPos.x - 50,
		uiPos.y + HP_IMAGE_SIZE.y / 2,
		0.173f,
		0.0f,
		uiImages.at((int)IMAGE_KINDS::ICON_BACK),
		true
	);
	DrawRotaGraph(
		uiPos.x - 50,
		uiPos.y + HP_IMAGE_SIZE.y / 2,
		0.173f,
		0.0f,
		uiImages.at((int)IMAGE_KINDS::ICON_CHARA),
		true
	);

	DrawRotaGraph(
		uiPos.x - 50,
		uiPos.y + HP_IMAGE_SIZE.y / 2,
		0.173f,
		0.0f,
		uiImages.at((int)IMAGE_KINDS::ICON_FRAME),
		true
	);
}

void CharacterHpUI::SubRelease()
{
}