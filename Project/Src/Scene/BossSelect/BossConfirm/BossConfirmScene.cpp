#include "BossConfirmScene.h"

#include "../../../pch.h"

#include "../../../Manager/Input/KeyManager.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../../Manager/Sound/SoundManager.h"

#include "../../SceneManager/SceneManager.h"

BossConfirmScene::BossConfirmScene() :
	SceneBase(),
	IS_HOST(Net::GetIns().IsHost()),

	choice(),

	buttonSelectionState(),

	readyList(),

	bossSelectImage(-1),

	choiceButtonImage(),

	arrowImage(-1),

	enterKeyImage(),

	arrowEasingCounter(0.0f),
	arrowEasingRate(0.0f)
{
}

void BossConfirmScene::SubPostLoad(void)
{
	// ホストは自分以外の全員、クライアントは自分のみの準備完了状態を管理する
	readyList.resize((int)Net::GetIns().GetConnectStatus().EntryCount(), (unsigned char)false);

#pragma region 各画像の読み込み

	// 画像読み込みのラムダ関数
	auto loadBossConfirmImage = [&](std::string name)->int { return LoadGraph((IMAGE_DATA_FILE_DIR + name + ".png").c_str()); };

	// ボスの詳細イメージ
	bossSelectImage = loadBossConfirmImage(BOSS_SELECT_IMAGE_NAME[(int)SceneManager::GetIns().GetSelectBossType()]);

	// 選択肢ボタンの画像
	for (int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {

		// 非選択時/選択時/選択不可 の画像を順番に読み込む
		for (int selectionState = 0; selectionState < (int)SELECTION_STATE::Max; selectionState++) {
			choiceButtonImage[choiceIndex][selectionState] =
				loadBossConfirmImage(CHOICE_BUTTON_IMAGE_NAME[choiceIndex] + CHOICE_BUTTON_IMAGE_DECORATION[selectionState]);
		}
	}

	// 選択中の矢印
	arrowImage = loadBossConfirmImage("BossConfirm/NowSelectArrow");

	// 選択中の決定キー
	enterKeyImage[(int)false] = loadBossConfirmImage("BossConfirm/NowSelectKeyboard");
	enterKeyImage[(int)true] = loadBossConfirmImage("BossConfirm/NowSelectController");

#pragma endregion
}

void BossConfirmScene::SubPostInit(void)
{
	// クライアントの準備完了状態を初期化
	for (unsigned char& ready : readyList) { ready = (unsigned char)false; }
	// ホストは常に準備完了状態
	readyList.at((int)Net::HOST_SENDER_ID) = (unsigned char)true;

	// 選択中の値を初期化
	choice = CHOICE::Enter;
	// ボタンごとの選択状態を初期化
	ButtonSelectionStateReload();
}

void BossConfirmScene::SubPostUpdate(void)
{
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
		if (choice > CHOICE::None && choice < CHOICE::Max &&
			buttonSelectionState[(int)choice] != SELECTION_STATE::Disable
			) {
			Snd::GetIns().Play("SystemButton");
		}
		else { /*Snd::GetIns().Play("");*/ }

		// 選択肢ごとの処理
		switch (choice) {

		case BossConfirmScene::CHOICE::None: { break; }	// 選択肢なし（ありえないはず）

		case BossConfirmScene::CHOICE::Exit: {	// キャンセル

			// クライアントかつ、準備完了状態だったら、処理をしない
			if (!IS_HOST && readyList.at((int)Net::GetIns().GetSenderId()) == (unsigned char)true) { break; }

			// キャンセル送信

			// 自身のシーン破棄
			SceneManager::GetIns().PopScene();

			break;
		}

		case BossConfirmScene::CHOICE::Enter: {	// <ホスト>出撃 / <クライアント>準備完了

			// ホスト
			if (IS_HOST) {

				// 全クライアントの準備完了フラグを確認する
				if (buttonSelectionState[(int)choice] == SELECTION_STATE::Disable) { break; }

				// ゲームシーン遷移を通知
				Net::GetIns().Send(MsgDataSystemInform(MsgDataSystemInform::INFORM_TYPE::ChangeSceneGame));

				// ゲームシーン遷移
				SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Game);

			}
			// クライアント
			else {
				// 自信の準備状態の参照を生成
				unsigned char& ready = readyList.at((int)Net::GetIns().GetSenderId());

				// 準備 完了/未完了 切り替え
				ready = 1 - ready;

				// 準備の状態を送信
				Net::GetIns().Send(MsgDataClientReady((bool)ready));

				// ボタンごとの選択状態を更新
				ButtonSelectionStateReload();
			}

			break;
		}
		}
	}

#pragma region イージング
	arrowEasingCounter += 0.1f;
	if (arrowEasingCounter > 100000.0f) { arrowEasingCounter = 0.0f; }
	arrowEasingRate = 1.0f + (sinf(arrowEasingCounter) * 0.15f);
#pragma endregion

	// 受信処理
	if (Net::GetIns().GetState() != Net::NetState::None) { ReceptionUpdate(); }
}

void BossConfirmScene::SubPostDraw(void)
{
	// 画面全体を半透明の黒で塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 画像描画のラムダ関数
	auto drawImage = [](int handle, const Vector2I& pos, float rate = 1.0f, float angle = 0.0f)->void { DrawRotaGraph(pos.x, pos.y, rate, angle, handle, true); };

	// ボス詳細イメージ
	drawImage(bossSelectImage, BOSS_SELECT_IMAGE_POS);

	// 選択肢の描画
	for (int i = 0; i < (int)CHOICE::Max; i++) {
		DrawRotaGraph(CHOICE_BUTTON_POS[i].x, CHOICE_BUTTON_POS[i].y, 1, 0, choiceButtonImage[i][(int)buttonSelectionState[i]], true);
	}
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y, arrowEasingRate, 0, arrowImage, true);
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y - 75, 0.3f, 0, enterKeyImage[(int)Key::GetIns().LastInputKinds()], true);
}

void BossConfirmScene::SubPreRelease(void)
{
#pragma region 画像を解放

	// 選択中の決定キー
	DeleteGraph(enterKeyImage[(int)true]);
	DeleteGraph(enterKeyImage[(int)false]);

	// 選択中の矢印
	DeleteGraph(arrowImage);

	// 選択肢ボタンの画像
	for (int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {
		for (int selectionState = 0; selectionState < (int)SELECTION_STATE::Max; selectionState++) {
			DeleteGraph(choiceButtonImage[choiceIndex][selectionState]);
		}
	}

	// ボス詳細イメージ
	DeleteGraph(bossSelectImage);

#pragma endregion
}

void BossConfirmScene::ButtonSelectionStateReload(void)
{
	// クライアントで準備完了を押していた場合、要素すべてを選択不可状態にする
	if (!IS_HOST && readyList.at((int)Net::GetIns().GetSenderId()) == (unsigned char)true) {
		for (auto& state : buttonSelectionState) { state = SELECTION_STATE::Disable; }
		return;
	}

	for (int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {
		// choice(選択中のボタンの種類) を参照し、
		// そのボタンを 選択中の場合「Select」/ 選択中の場合「NotSelect」
		buttonSelectionState[choiceIndex] = ((int)choice == choiceIndex) ? SELECTION_STATE::Select : SELECTION_STATE::NotSelect;
	}

	// ホストの場合

	// 全クライアントの準備完了フラグを確認する
	// まだ全員準備完了していなかったら、出撃ボタンを選択不可状態にする
	for (int id = 0; id < (int)Net::GetIns().GetConnectStatus().EntryCount(); id++) {

		// 準備完了していないクライアントがいたら、出撃ボタンを選択不可状態にする
		if (readyList.at(id) == (unsigned char)false) {
			buttonSelectionState[(int)CHOICE::Enter] = SELECTION_STATE::Disable;
			break;
		}
	}
}

void BossConfirmScene::ReceptionUpdate(void)
{

}