#include "LobbyScene.h"

#include "../../Manager/Input/KeyManager.h"
#include "../../Manager/Camera/Camera.h"

#include "../../Manager/Net/NetWorkManager.h"
#include "../../Manager/Sound/SoundManager.h"

#include "../../Scene/SceneManager/SceneManager.h"

#include "CharaSelect/CharaSelectScene.h"
#include "BossSelect/BossSelectScene.h"
#include "MultiPopup/MultiPopupScene.h"

#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/Lobby/LobbyStage/LobbyStage.h"
#include "../../Object/Lobby/LobbyCharaPreview/LobbyCharaPreviewManager.h"
#include "../../Object/Lobby/LobbyBossPreview/LobbyBossPreview.h"


LobbyScene::LobbyScene()
{
}

void LobbyScene::Load(void)
{
	if (SceneManager::GetIns().GetSelectBossType() == BOSS_TYPE::None) {
		SceneManager::GetIns().SetSelectBossType((BOSS_TYPE)((int)BOSS_TYPE::None + 1));
	}

	// 初期化も含めたオブジェクト生成のラムダ関数
	auto ObjAdd = [&](ActorBase* newClass)->void {
		// 配列の末尾に追加
		objects.emplace_back(newClass);
		// 共通の読み込み処理
		objects.back()->Load();
		};

	// オブジェクト生成（生成の順番がそのまま(更新/描画)順）
	//<例>ObjAdd(new Player());

	ObjAdd(new SkyDome());
	ObjAdd(new LobbyStage());
	ObjAdd(new LobbyCharaPreviewManager());
	ObjAdd(new LobbyBossPreview());

#pragma region 各画像の読み込み

	// 画面上部のボードの画像
	boardImage = LoadGraph((IMAGE_DATA_FILE_DIR + "LobbyBoard.png").c_str());

	// 選択肢ボタンの画像
	for (int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {
		// 名前
		const std::string IMAGE_NAME = IMAGE_DATA_FILE_DIR + CHOICE_BUTTON_IMAGE_NAME[choiceIndex];

		// 選択時
		choiceButtonImage[choiceIndex][(int)true] = LoadGraph((IMAGE_NAME + CHOICE_BUTTON_IMAGE_DECORATION + ".png").c_str());

		// 非選択時
		choiceButtonImage[choiceIndex][(int)false] = LoadGraph((IMAGE_NAME + NOT_CHOICE_BUTTON_IMAGE_DECORATION + ".png").c_str());
	}

	// 選択中の矢印
	arrowImage = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectArrow.png").c_str());

	// 選択中の決定キー
	enterKeyImage[(int)false] = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectKeyboard.png").c_str());
	enterKeyImage[(int)true] = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectController.png").c_str());

#pragma endregion
}

void LobbyScene::Init(void)
{
	SetFogEnable(false);

	// オブジェクト全ての初期化処理
	for (ActorBase* obj : objects) { obj->Init(); }

	choice = CHOICE::CharaChange;

	Camera::GetIns().ChangeModeFixedPoint(Vector3(330, 530, -1000), Vector3::Xonly(Deg2Rad(16)));
}

void LobbyScene::Update(void)
{
	// オブジェクト全ての更新処理
	for (ActorBase* obj : objects) { obj->Update(); }

#pragma region 操作

	// タイトル画面に戻る
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {
		Snd::GetIns().Play("SystemButton");
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::TITLE);
		return;
	}

	// 左
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		Snd::GetIns().Play("SystemSelect");

		choice = (CHOICE)((int)choice - 1);
		if (choice <= CHOICE::None) { choice = (CHOICE)((int)CHOICE::None + 1); }
	}
	// 右
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		Snd::GetIns().Play("SystemSelect");

		choice = (CHOICE)((int)choice + 1);
		if (choice >= CHOICE::Max) { choice = (CHOICE)((int)CHOICE::Max - 1); }
	}


	// 決定
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
		Snd::GetIns().Play("SystemButton");

		// 選択肢ごとの処理
		switch (choice) {

		case LobbyScene::CHOICE::None: { break; }	// 選択肢なし（ありえないはず）

		case LobbyScene::CHOICE::Exit: {	// 退出

			// ネットワーク切断
			Net::GetIns().Disconnection();

			// タイトル画面に戻る
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::TITLE);

			// 以降はthisがnullptrとなっているため終了
			return;
		}

		case LobbyScene::CHOICE::Multi: {	// マルチ

			// 専用のシーンを追加する
			SceneManager::GetIns().PushScene(std::make_shared<MultiPopupScene>());

			// 終了
			return;
		}

		case LobbyScene::CHOICE::BossChange: {	// ボス変更

			// 専用のシーンを追加する
			SceneManager::GetIns().PushScene(
				std::make_shared<BossSelectScene>(
					// ボス変更シーンから戻ってきたときに、プレビューを更新
					[&]() { ObjSerch<LobbyBossPreview>()->SetSelectBossType(SceneManager::GetIns().GetSelectBossType()); }
				)
			);

			// 終了
			return;
		}

		case LobbyScene::CHOICE::CharaChange: {	// キャラ変更

			// 専用のシーンを追加する
			SceneManager::GetIns().PushScene(
				std::make_shared<CharaSelectScene>(
					// キャラ変更シーンから戻ってきたときに、プレビューを更新
					[&]() { ObjSerch<LobbyCharaPreviewManager>()->ReloadChara(); }
				)
			);

			// 終了
			return;
		}

		case LobbyScene::CHOICE::Enter: {	// 出撃

			Net::GetIns().StartHost();
			Net::GetIns().CloseReceptionToConnected();

			// ゲームシーンに遷移
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GAME);

			// 以降はthisがnullptrとなっているため終了
			return;
		}
		}
	}

#pragma endregion

}

void LobbyScene::Draw(void)
{
	// オブジェクト全ての描画処理
	for (ActorBase* obj : objects) { obj->Draw(); }
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	for (ActorBase* obj : objects) { obj->AlphaDraw(); }
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	for (ActorBase* obj : objects) { obj->UiDraw(); }

	// ボードの描画
	DrawRotaGraph(App::SCREEN_SIZE_X_HALF, 115, 1, 0, boardImage, true);

	// 選択肢の描画
	for(int i = 0; i < (int)CHOICE::Max; i++) {
		DrawRotaGraph(CHOICE_BUTTON_POS[i].x, CHOICE_BUTTON_POS[i].y, 1, 0, choiceButtonImage[i][(int)((CHOICE)i == choice)], true);
	}
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y, 1, 0, arrowImage, true);
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y - 75, 0.3f, 0, enterKeyImage[(int)Key::GetIns().LastInputKinds()], true);

}

void LobbyScene::Release(void)
{
	// 画像の解放
	DeleteGraph(enterKeyImage[0]);
	DeleteGraph(enterKeyImage[1]);
	DeleteGraph(arrowImage);
	for (int* handlePair : choiceButtonImage) {
		DeleteGraph(handlePair[0]);
		DeleteGraph(handlePair[1]);
	}
	DeleteGraph(boardImage);

	// オブジェクト全ての解放処理
	for (ActorBase*& obj : objects) {
		if (!obj) { continue; }
		obj->Release();
		delete obj;
		obj = nullptr;
	}

	SetFogEnable(true);
}
