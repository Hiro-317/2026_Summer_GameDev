#pragma once

#include "../../ActorBase.h"

#include "../../../Application/Application.h"

#include "../../CharaTypeDefine.h"

class LobbyBossPreview : public ActorBase
{
public:
	LobbyBossPreview();
	~LobbyBossPreview()override = default;

	void Load(void)override;
	void Init(void)override {}
	void Update(void)override {}
	void Draw(void)override {}
	void AlphaDraw(void)override {}
	void UiDraw(void)override;
	void Release(void)override;

	void SetSelectBossType(BOSS_TYPE selectBossType);

private:
#pragma region 定数定義

	// 枠の画像パス
	const std::string FRAME_IMAGE_PATH = "Data/Image/Lobby/SelectBossPreviewFrame.png";

	// ボス選択の画像パス配列
	const std::string BOSS_SELECT_IMAGE_PATH[(int)BOSS_TYPE::Max] = {
		"Data/Image/Lobby/BossSelect/TomatoBossSelectImage.png",
		"Data/Image/Lobby/BossSelect/GrapeBossSelectImage.png",
	};

	// 枠の画像の座標
	const Vector2I FRAME_IMAGE_POS = Vector2I(App::SCREEN_SIZE_X - 200, 150);

	// 選択中ボスの画像の座標
	const Vector2I BOSS_SELECT_IMAGE_POS = FRAME_IMAGE_POS + Vector2I(-1,15);

#pragma endregion

	// 枠の画像
	int frameImage;

	// 選択中ボスの画像
	int bossSelectImage;
};