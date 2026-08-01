#pragma once

#include <string>
#include <functional>

#include "../../SceneBase.h"

#include "../../../Application/Application.h"

#include "../../../Object/CharaTypeDefine.h"

#include "../../../Common/Vector2.h"

class BossSelectScene : public SceneBase
{
public:
	BossSelectScene(
		const std::function<void(void)>& LobbyPreviewBossChange,
		const std::function<void(void)>& LobbyReceptionUpdate = nullptr
	);
	~BossSelectScene()override = default;

private:

#pragma region 主要関数再定義

	// 読み込み
	void SubPostLoad(void)override;

	// 初期化
	void SubPostInit(void)override;

	// 更新
	void SubPostUpdate(void)override;

	// 描画
	void SubPostDraw(void)override;

	// 終了
	void SubPreRelease(void);

#pragma endregion

	bool IsUseLoadingScreen(void)const override { return false; }

	// カメラは使用しない
	bool UseCamera(void)const override { return false; }

	// 当たり判定管理は使用しない
	bool UseCollisionManager(void)const override { return false; }

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

	// マルチロビー画面の受信処理を受け取る
	const std::function<void(void)> LobbyReceptionUpdate;
};