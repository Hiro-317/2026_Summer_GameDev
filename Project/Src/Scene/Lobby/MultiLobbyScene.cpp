#include "MultiLobbyScene.h"

#include "../../Manager/Input/KeyManager.h"
#include "../../Manager/Camera/Camera.h"

#include "../../Manager/Net/NetWorkManager.h"
#include "../../Manager/Sound/SoundManager.h"

#include "../../Scene/SceneManager/SceneManager.h"

#include "CharaSelect/CharaSelectScene.h"

#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/Lobby/LobbyStage/LobbyStage.h"
#include "../../Object/Lobby/LobbyCharaPreview/LobbyCharaPreviewManager.h"

MultiLobbyScene::MultiLobbyScene() : 
	SceneBase(),
	CHOICE_BUTTON_IMAGE_NAME(
		"LobbyDisconnected",
		"LobbyCharaChange",
		(Net::GetIns().IsHost()) ? "LobbyEnter" : "LobbyClientEnter"
	)
{
}

void MultiLobbyScene::Load(void)
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

	// ホストは自分以外の全員、クライアントは自分のみの準備完了状態を管理する
	clientReady.resize((Net::GetIns().IsHost()) ? (int)MSG_SENDER_ID::Max - 1 : 1);

#pragma region 各画像の読み込み

	// 画面上部のボードの画像
	boardImage = LoadGraph((IMAGE_DATA_FILE_DIR + "LobbyBoard.png").c_str());

	// 選択肢ボタンの画像
	for (int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {
		// 名前
		const std::string IMAGE_NAME = IMAGE_DATA_FILE_DIR + CHOICE_BUTTON_IMAGE_NAME[choiceIndex];

		// 非選択時/選択時/選択不可 の画像を順番に読み込む
		for (int selectionState = 0; selectionState < (int)SELECTION_STATE::Max; selectionState++) {
			choiceButtonImage[choiceIndex][selectionState] =
				LoadGraph((IMAGE_NAME + CHOICE_BUTTON_IMAGE_DECORATION[selectionState] + ".png").c_str());
		}
	}

	// 選択中の矢印
	arrowImage = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectArrow.png").c_str());

	// 選択中の決定キー
	enterKeyImage[(int)false] = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectKeyboard.png").c_str());
	enterKeyImage[(int)true] = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectController.png").c_str());

#pragma endregion
}

void MultiLobbyScene::Init(void)
{
	SetFogEnable(false);

	// オブジェクト全ての初期化処理
	for (ActorBase* obj : objects) { obj->Init(); }

	// クライアントの準備完了状態を初期化
	for (size_t i = 0; i < clientReady.size(); i++) { clientReady.at(i) = false; }

	for (unsigned char& ready : clientReady) { ready = (unsigned char)false; }

	// 選択中の値を初期化
	choice = CHOICE::CharaChange;
	// ボタンごとの選択状態を初期化
	ButtonSelectionStateReload();

	// カメラの設定
	Camera::GetIns().ChangeModeFixedPoint(Vector3(330, 530, -1000), Vector3::Xonly(Deg2Rad(16)));
}

void MultiLobbyScene::Update(void)
{
	// オブジェクト全ての更新処理
	for (ActorBase* obj : objects) { obj->Update(); }

#pragma region 操作

	// タイトル画面に戻る
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

		// 効果音
		Snd::GetIns().Play("SystemButton");

		// ネットワーク切断
		Net::GetIns().Disconnection();

		// ロビー画面に戻る
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::LOBBY);
		return;
	}

	// 左
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {

		// 効果音
		Snd::GetIns().Play("SystemSelect");

		// 1つ左へ（選択中の値を1つ減らす）
		choice = (CHOICE)((int)choice - 1);
		// 範囲を超えていたら戻す
		if (choice <= CHOICE::None) { choice = (CHOICE)((int)CHOICE::None + 1); }

		// ボタンごとの選択状態を更新
		ButtonSelectionStateReload();
	}
	// 右
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {

		// 効果音
		Snd::GetIns().Play("SystemSelect");

		// 1つ右へ（選択中の値を1つ増やす)）
		choice = (CHOICE)((int)choice + 1);
		// 範囲を超えていたら戻す
		if (choice >= CHOICE::Max) { choice = (CHOICE)((int)CHOICE::Max - 1); }

		// ボタンごとの選択状態を更新
		ButtonSelectionStateReload();
	}

	// 決定
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

		// 効果音
		Snd::GetIns().Play("SystemButton");

		// 選択肢ごとの処理
		switch (choice) {

		case MultiLobbyScene::CHOICE::None: { break; }	// 選択肢なし（ありえないはず）

		case MultiLobbyScene::CHOICE::Disconnected: {	// 切断

			// ネットワーク切断
			Net::GetIns().Disconnection();

			// 通常のロビー画面に戻る
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::LOBBY);

			// 以降はthisがnullptrとなっているため終了
			return;
		}

		case MultiLobbyScene::CHOICE::CharaChange: {	// キャラ変更

			// 専用のシーンを追加する
			SceneManager::GetIns().PushScene(
				std::make_shared<CharaSelectScene>(
					// キャラ変更シーンから戻ってきたときに、プレビューを更新
					[&]() { ObjSerch<LobbyCharaPreviewManager>()->ReloadChara(Net::GetIns().GetSenderId()); }
				)
			);

			// このシーンの処理は終了
			return;
		}

		case MultiLobbyScene::CHOICE::Enter: {	// <ホスト>出撃 / <クライアント>準備完了

			// ホスト
			if (Net::GetIns().IsHost()) {
				// ゲームシーン遷移を通知
				Net::GetIns().Send(MsgDataSystemInform(MsgDataSystemInform::INFORM_TYPE::ChangeSceneGame));

				// ゲームシーン遷移
				SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GAME);

				// 以降はthisがnullptrとなっているため終了
				return;
			}
			// クライアント
			else {
				// 準備完了
				clientReady.back() = 1 - clientReady.back();

				// 準備完了の状態を送信
				Net::GetIns().Send(MsgDataClientReady((bool)clientReady.back()));

				// ボタンごとの選択状態を更新
				ButtonSelectionStateReload();
			}
		}
		}
	}

#pragma endregion

#pragma region 受信処理

	// 選択キャラの受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataCharaSelect>()) {

		// 受け取ったキャラタイプを保存する
		SceneManager::GetIns().SetSelectCharaType(dataPtr->header.senderId, (CHARA_TYPE)dataPtr->charaType);

		// キャラプレビューを更新する
		ObjSerch<LobbyCharaPreviewManager>()->ReloadChara(dataPtr->header.senderId);

		delete dataPtr;
	}

	// 接続完了の受信
	while(auto dataPtr = Net::GetIns().GetMsgData<MsgDataClientReady>()) {

		// 受け取った準備完了フラグを保存する
		clientReady.at((int)dataPtr->header.senderId - 1) = (unsigned char)dataPtr->ready;

		delete dataPtr;
	}

	
	// システム通知の受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>()) {
		
		// シーン遷移の受信
		if (dataPtr->inform == MsgDataSystemInform::INFORM_TYPE::ChangeSceneGame) {
			// ゲームシーン遷移
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GAME);

			delete dataPtr;
			return;
		}

		delete dataPtr;
	}

#pragma endregion

}

void MultiLobbyScene::Draw(void)
{
	// オブジェクト全ての描画処理
	for (ActorBase* obj : objects) { obj->Draw(); }

	// ボードの描画
	DrawRotaGraph(BOARD_POS.x, BOARD_POS.y, 1, 0, boardImage, true);

	// 選択肢の描画
	for (int i = 0; i < (int)CHOICE::Max; i++) {
		DrawRotaGraph(CHOICE_BUTTON_POS[i].x, CHOICE_BUTTON_POS[i].y, 1, 0, choiceButtonImage[i][(int)buttonSelectionState[i]], true);
	}
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y, 1, 0, arrowImage, true);
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y - 75, 1, 0, enterKeyImage[(int)Key::GetIns().LastInputKinds()], true);

}

void MultiLobbyScene::Release(void)
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

void MultiLobbyScene::ButtonSelectionStateReload(void)
{
	for (int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {
		// クライアントで準備完了を押していた場合、要素すべてを選択不可状態にする
		if (!Net::GetIns().IsHost()) {
			if (clientReady.back() == (unsigned char)true) { buttonSelectionState[choiceIndex] = SELECTION_STATE::Disable; continue; }
		}

		// 上記の例外を除き、

		// choice(選択中のボタンの種類) を参照し、
		// そのボタンを 選択中の場合「Select」/ 選択中の場合「NotSelect」
		buttonSelectionState[choiceIndex] =
			((int)choice == choiceIndex) ? SELECTION_STATE::Select : SELECTION_STATE::NotSelect;
	}
}
