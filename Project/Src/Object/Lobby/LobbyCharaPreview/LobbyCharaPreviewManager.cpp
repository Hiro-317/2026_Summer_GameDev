#include "LobbyCharaPreviewManager.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../../Scene/SceneManager/SceneManager.h"

#include "Orange/LobbyCharaPreviewOrange.h"

LobbyCharaPreviewManager::LobbyCharaPreviewManager() :
	charaPreview(nullptr, nullptr, nullptr, nullptr)
{
}

void LobbyCharaPreviewManager::Load(void)
{
	if(SceneManager::GetIns().GetSelectCharaType(Net::HOST_SENDER_ID) == CHARA_TYPE::None) {
		SceneManager::GetIns().SetSelectCharaType(Net::HOST_SENDER_ID, CHARA_TYPE::Orange);
	}
	ReloadChara(Net::HOST_SENDER_ID);
}

void LobbyCharaPreviewManager::Update(void)
{
	// キャラプレビューの更新
	for (LobbyCharaPreviewBase* preview : charaPreview) {
		if (preview == nullptr) { continue; }
		preview->Update();
	}
}

void LobbyCharaPreviewManager::Draw(void)
{
	// キャラプレビューの描画
	for (LobbyCharaPreviewBase* preview : charaPreview) {
		if (preview == nullptr) { continue; }
		preview->Draw();
	}
}

void LobbyCharaPreviewManager::Release(void)
{
	// キャラプレビューの解放
	for (LobbyCharaPreviewBase*& preview : charaPreview) {
		if (preview == nullptr) { continue; }
		preview->Release();
		delete preview;
		preview = nullptr;
	}
}

void LobbyCharaPreviewManager::ReloadChara(MSG_SENDER_ID senderId)
{
	// 送信元IDのチェック
	if (senderId <= MSG_SENDER_ID::None || MSG_SENDER_ID::Max <= senderId) { return; }

	// キャラプレビューの解放
	if (charaPreview[(int)senderId] != nullptr) {
		charaPreview[(int)senderId]->Release();
		delete charaPreview[(int)senderId];
		charaPreview[(int)senderId] = nullptr;
	}

	// キャラプレビューの生成
	switch (SceneManager::GetIns().GetSelectCharaType(senderId)) {

	case CHARA_TYPE::None: { break; }	// 未選択

	case CHARA_TYPE::Orange: {	// オレンジ
		charaPreview[(int)senderId] = new LobbyCharaPreviewOrange(CHARA_PREVIEW_POS[(int)senderId]);
		break;
	}

	case CHARA_TYPE::Tomato: {	// トマト
		charaPreview[(int)senderId] = new LobbyCharaPreviewOrange(CHARA_PREVIEW_POS[(int)senderId]);
		break;
	}

	default: { break; }	// 例外
	}

	// キャラプレビューのロードと初期化
	charaPreview[(int)senderId]->Load();
	charaPreview[(int)senderId]->Init();
}