#include "CharaSelectScene.h"

#include "../../../Application/Application.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../SceneManager/SceneManager.h"

#include "../../../Object/Lobby/CharaSelectScene/CharaSelectPreviewManager.h"

CharaSelectScene::CharaSelectScene(const std::function<void(void)>& LobbyPreviewCharaChange) :
	LobbyPreviewCharaChange(LobbyPreviewCharaChange)
{
}

void CharaSelectScene::Load(void)
{
	// 初期化も含めたオブジェクト生成のラムダ関数
	auto ObjAdd = [&](ActorBase* newClass)->void {
		// 配列の末尾に追加
		objects.emplace_back(newClass);
		// 共通の読み込み処理
		objects.back()->Load();
		};

	ObjAdd(new CharaSelectPreviewManager());
}

void CharaSelectScene::Init(void)
{
	// 初期化
	for (ActorBase* obj : objects) { obj->Init(); }
}

void CharaSelectScene::Update(void)
{
	// 更新
	for (ActorBase* obj : objects) { obj->Update(); }

	// 戻る
	if(Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

		// このシーンを破棄
		SceneManager::GetIns().PopScene();

		// 以降はthisがnullptrとなっているため終了
		return;
	}

	// キャラ確定
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

		// 現在CharaSelectPreviewManagerクラスで選択されているキャラを取得
		CHARA_TYPE nowSelect = ObjSerch<CharaSelectPreviewManager>()->GetCharaType();

		if (nowSelect == CHARA_TYPE::None) { return; }

		// 選択中のキャラで、SceneManagerが抱えているキャラ選択情報を書き換える
		SceneManager::GetIns().SetSelectCharaType(Net::GetIns().GetSenderId() == MSG_SENDER_ID::None ? MSG_SENDER_ID::P1 : Net::GetIns().GetSenderId(), nowSelect);

		// キャラ変更情報を送信
		Net::GetIns().Send(MsgDataCharaSelect((int)nowSelect));

		// このパソコンのロビーシーンのプレビューを更新する
		LobbyPreviewCharaChange();

		// キャラ変更終了でこのシーンを破棄
		SceneManager::GetIns().PopScene();

		// 以降はthisがnullptrとなっているため終了
		return;
	}
}

void CharaSelectScene::Draw(void)
{
	// 画面全体を半透明の黒で塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (ActorBase* obj : objects) { obj->Draw(); }
}

void CharaSelectScene::Release(void)
{
	for (ActorBase*& obj : objects) {
		obj->Release();
		delete obj;
		obj = nullptr;
	}
	objects.clear();
}
