#include "LobbyCharaPreviewManager.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../../Scene/SceneManager/SceneManager.h"

#include "Orange/LobbyCharaPreviewOrange.h"

LobbyCharaPreviewManager::LobbyCharaPreviewManager():
	charaPreview(nullptr)
{
}

void LobbyCharaPreviewManager::Load(void)
{
	if(SceneManager::GetIns().GetSelectCharaType(Net::HOST_SENDER_ID) == CHARA_TYPE::None) {
		SceneManager::GetIns().SetSelectCharaType(Net::HOST_SENDER_ID, CHARA_TYPE::Orange);
	}
	ChangeChara(SceneManager::GetIns().GetSelectCharaType(Net::HOST_SENDER_ID));
}

void LobbyCharaPreviewManager::Init(void)
{
}

void LobbyCharaPreviewManager::Update(void)
{
	// キャラプレビューの更新
	charaPreview->Update();
}

void LobbyCharaPreviewManager::Draw(void)
{
	// キャラプレビューの描画
	charaPreview->Draw();
}

void LobbyCharaPreviewManager::Release(void)
{
	// キャラプレビューの解放
	if(charaPreview != nullptr) {
		charaPreview->Release();
		delete charaPreview;
		charaPreview = nullptr;
	}
}

void LobbyCharaPreviewManager::ChangeChara(CHARA_TYPE type)
{
	// キャラプレビューの解放
	if(charaPreview != nullptr) {
		charaPreview->Release();
		delete charaPreview;
		charaPreview = nullptr;
	}

	switch (type) {
	case CHARA_TYPE::None: { break; }

	case CHARA_TYPE::Orange: {
		charaPreview = new LobbyCharaPreviewOrange(Vector3(300, 0, 100));
		break;
	}
	case CHARA_TYPE::Tomato: {
		charaPreview = new LobbyCharaPreviewOrange(Vector3(300, 0, 100));
		break;
	}
	case CHARA_TYPE::Tanaka: {
		charaPreview = new LobbyCharaPreviewOrange(Vector3(300, 0, 100));
		break;
	}
	default: { break; }
	}

	if(charaPreview != nullptr) {
		charaPreview->Load();
		charaPreview->Init();
	}
}
