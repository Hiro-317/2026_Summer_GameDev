#include "CharaSelectScene.h"

#include "../../../Application/Application.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../SceneManager/SceneManager.h"

#include "../../../Object/CharaSelect/CharaSelectPreviewManager.h"

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
	for (ActorBase* obj : objects) { obj->Init(); }
}

void CharaSelectScene::Update(void)
{
	for (ActorBase* obj : objects) { obj->Update(); }

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
		if (ObjSerch<CharaSelectPreviewManager>()->GetCharaType() != CHARA_TYPE::Orange) { return; }
		SceneManager::GetIns().PopScene();
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
	SceneManager::GetIns().SetSelectCharaType(Net::HOST_SENDER_ID, (ObjSerch<CharaSelectPreviewManager>())->GetCharaType());
	LobbyPreviewCharaChange();

	for (ActorBase*& obj : objects) {
		obj->Release();
		delete obj;
		obj = nullptr;
	}
	objects.clear();
}
