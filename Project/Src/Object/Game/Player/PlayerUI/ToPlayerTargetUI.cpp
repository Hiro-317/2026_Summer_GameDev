#include "ToPlayerTargetUI.h"

ToPlayerTargetUI::ToPlayerTargetUI(const std::array<Vector2I, (size_t)MSG_SENDER_ID::Max>& DRAW_POS_TABLE, const unsigned char& targetPlayerIndex) :
	ActorBase(),

	DRAW_POS_TABLE(DRAW_POS_TABLE),

	targetPlayerIndex(targetPlayerIndex),

	selectedFrameImage(-1)
{
}

void ToPlayerTargetUI::Load(void)
{
	// 画像を読み込む（選択中フレーム）
	selectedFrameImage = LoadGraph("Data/Image/Game/UI/Player/PlayerToNowSelect.png");
}

void ToPlayerTargetUI::SubInit(void)
{
	// UI描画に設定する
	SetDrawType(ACTOR_DRAW_TYPE::Ui);
}

void ToPlayerTargetUI::SubDraw(void)
{
	// 描画（選択中フレーム）
	DrawRotaGraph(DRAW_POS_TABLE.at(targetPlayerIndex).x, DRAW_POS_TABLE.at(targetPlayerIndex).y, 1, 0, selectedFrameImage, true);
}	

void ToPlayerTargetUI::SubRelease(void)
{
	// 画像を解放（選択中フレーム）
	if (selectedFrameImage != -1) {
		DeleteGraph(selectedFrameImage);
		selectedFrameImage = -1;
	}
}
