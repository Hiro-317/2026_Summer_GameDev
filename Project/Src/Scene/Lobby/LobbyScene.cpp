#include "LobbyScene.h"

#include "../../Manager/Input/KeyManager.h"
#include "../../Manager/Camera/Camera.h"

#include "../../Manager/Net/NetWorkManager.h"
#include "../../Manager/Sound/SoundManager.h"

#include "../../Scene/SceneManager/SceneManager.h"

#include "CharaSelect/CharaSelectScene.h"

#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/Lobby/LobbyStage/LobbyStage.h"
#include "../../Object/Lobby/LobbyCharaPreview/LobbyCharaPreviewManager.h"

LobbyScene::LobbyScene()
{
}

void LobbyScene::Load(void)
{
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

#pragma region 各画像の読み込み
	boardImage = LoadGraph("Data/Image/Lobby/LobbyBoard.png");

	choiceButtonImage[(int)CHOICE::Exit][0] = LoadGraph("Data/Image/Lobby/LobbyExitToNotSelect.png");
	choiceButtonImage[(int)CHOICE::Exit][1] = LoadGraph("Data/Image/Lobby/LobbyExitToSelect.png");

	choiceButtonImage[(int)CHOICE::CharaChange][0] = LoadGraph("Data/Image/Lobby/LobbyCharaChangeToNotSelect.png");
	choiceButtonImage[(int)CHOICE::CharaChange][1] = LoadGraph("Data/Image/Lobby/LobbyCharaChangeToSelect.png");

	choiceButtonImage[(int)CHOICE::Enter][0] = LoadGraph("Data/Image/Lobby/LobbyEnterToNotSelect.png");
	choiceButtonImage[(int)CHOICE::Enter][1] = LoadGraph("Data/Image/Lobby/LobbyEnterToSelect.png");

	arrowImage = LoadGraph("Data/Image/Lobby/NowSelectArrow.png");

	enterKeyImage[0] = LoadGraph("Data/Image/Lobby/NowSelectKeyboard.png");
	enterKeyImage[1] = LoadGraph("Data/Image/Lobby/NowSelectController.png");
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

		switch (choice) {
		case LobbyScene::CHOICE::None: { break; }
		case LobbyScene::CHOICE::Exit: {
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::TITLE);
			return;
		}
		case LobbyScene::CHOICE::CharaChange: {
			SceneManager::GetIns().PushScene(
				std::make_shared<CharaSelectScene>(
					[&]() { ObjSerch< LobbyCharaPreviewManager>()->ChangeChara(SceneManager::GetIns().GetSelectCharaType(Net::HOST_SENDER_ID)); }
				)
			);
			return;
		}
		case LobbyScene::CHOICE::Enter: {
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::BOSS_SELECT);
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

	// ボードの描画
	DrawRotaGraph(App::SCREEN_SIZE_X_HALF, 115, 1, 0, boardImage, true);

	// 選択肢の描画
	for(int i = 0; i < (int)CHOICE::Max; i++) {
		DrawRotaGraph(choiceButtonPos[i].x, choiceButtonPos[i].y, 1, 0, choiceButtonImage[i][(int)((CHOICE)i == choice)], true);
	}
	DrawRotaGraph(choiceButtonPos[(int)choice].x, choiceButtonPos[(int)choice].y, 1, 0, arrowImage, true);
	DrawRotaGraph(choiceButtonPos[(int)choice].x, choiceButtonPos[(int)choice].y - 100, 1, 0, enterKeyImage[(int)Key::GetIns().LastInputKinds()], true);

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
	for (ActorBase* obj : objects) {
		if (!obj) { continue; }
		obj->Release();
		delete obj;
		obj = nullptr;
	}

	SetFogEnable(true);
}
