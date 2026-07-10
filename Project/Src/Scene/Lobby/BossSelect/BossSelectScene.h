#pragma once

#include <functional>

#include "../../SceneBase.h"

#include "../../../Application/Application.h"

#include "../../../Object/CharaTypeDefine.h"

class BossSelectScene : public SceneBase
{
public:
	BossSelectScene(const std::function<void(void)>& LobbyPreviewBossChange);
	~BossSelectScene()override = default;

	// 読み込み
	void Load(void)override;
	// 初期化処理
	void Init(void)override;
	// 更新ステップ
	void Update(void)override;
	// 描画処理
	void Draw(void)override;
	// 解放処理
	void Release(void)override;

private:

#pragma region 定数定義

	// ボス選択の画像パス配列
	const std::string BOSS_SELECT_IMAGE_PATH[(int)BOSS_TYPE::Max] = {
		"Data/Image/Lobby/BossSelect/TomatoBossSelectImage.png",
		"Data/Image/Lobby/BossSelect/GrapeBossSelectImage.png",
	};

	// 矢印の画像パス
	const std::string ARROW_IMAGE_PATH = "Data/Image/Lobby/BossSelect/BossSelectArrow.png";

	// 決定キー(キーボード)の画像パス
	const std::string KEYBOARD_ENTER_KEY_IMAGE_PATH = "Data/Image/Lobby/BossSelect/BossSelectEnterKeyboard.png";
	// 決定キー(コントローラー)の画像パス
	const std::string CONTROLLER_ENTER_KEY_IMAGE_PATH = "Data/Image/Lobby/BossSelect/BossSelectEnterController.png";

	// とじるキー(キーボード)の画像パス
	const std::string KEYBOARD_EXIT_KEY_IMAGE_PATH = "Data/Image/Lobby/ExitKeyboard.png";
	// とじるキー(コントローラー)の画像パス
	const std::string CONTROLLER_EXIT_KEY_IMAGE_PATH = "Data/Image/Lobby/ExitController.png";


	// ボス選択の画像の座標
	const Vector2I BOSS_SELECT_IMAGE_POS = Vector2I(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF);

	// 矢印の画像の座標
	const Vector2I ARROW_IMAGE_POS = BOSS_SELECT_IMAGE_POS;

	// 決定キーの画像の座標
	const Vector2I ENTER_IMAGE_POS = BOSS_SELECT_IMAGE_POS + Vector2I(0, 350);

	// とじるキーの画像の座標
	const Vector2I EXIT_IMAGE_POS = BOSS_SELECT_IMAGE_POS - Vector2I(525, 350);
#pragma endregion

	// ボス選択の画像配列
	int bossSelectImage[(int)BOSS_TYPE::Max];

	// 矢印の画像
	int arrowImage;

	// 決定キーの画像
	int enterImage[2];

	// とじるキーの画像
	int exitImage[2];

	// 拡大演出カウンター
	float easingCounter;
	// 拡大演出の最終的な倍率
	float easingRate;

	// 選択中のキャラタイプ
	BOSS_TYPE selectBossType;

	const std::function<void(void)> LobbyPreviewBossChange;
};