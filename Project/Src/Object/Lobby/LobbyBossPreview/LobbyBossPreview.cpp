#include "LobbyBossPreview.h"

#include "../../../Scene/SceneManager/SceneManager.h"

LobbyBossPreview::LobbyBossPreview() :
	ActorBase(),

	frameImage(-1),
	bossSelectImage(-1)
{
}

void LobbyBossPreview::Load(void)
{
	// フレーム画像をロードする
	frameImage = LoadGraph(FRAME_IMAGE_PATH.c_str());

	// 選択されたボスの種類に応じて画像をロードする
	SetSelectBossType(SceneManager::GetIns().GetSelectBossType());
}

void LobbyBossPreview::UiDraw(void)
{
	// 画像描画用ラムダ関数
	auto DrawImageToVector2I = [&](Vector2I pos, int imageHandle, float scale = 1.0f)->void {
		DrawRotaGraph(pos.x, pos.y, scale, 0, imageHandle, true);
		};

	DrawImageToVector2I(FRAME_IMAGE_POS, frameImage);

	static Vector2I tempPos = Vector2I();
	if (CheckHitKey(KEY_INPUT_UP)) { tempPos.y--; }
	if (CheckHitKey(KEY_INPUT_DOWN)) { tempPos.y++; }
	if (CheckHitKey(KEY_INPUT_LEFT)) { tempPos.x--; }
	if (CheckHitKey(KEY_INPUT_RIGHT)) { tempPos.x++; }

	DrawImageToVector2I(BOSS_SELECT_IMAGE_POS, bossSelectImage, 0.23f);
}

void LobbyBossPreview::Release(void)
{
	DeleteGraph(frameImage);
	DeleteGraph(bossSelectImage);
}

void LobbyBossPreview::SetSelectBossType(BOSS_TYPE selectBossType)
{
	// BOSS_TYPEの範囲外の値が渡された場合は処理を中断する
	if (selectBossType <= BOSS_TYPE::None || BOSS_TYPE::Max <= selectBossType) { return; }

	// 選択されたボスの画像をロードする
	bossSelectImage = LoadGraph(BOSS_SELECT_IMAGE_PATH[(int)selectBossType].c_str());
}
