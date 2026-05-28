#include "LobbyScene.h"

#include "../../Manager/Camera/Camera.h"

#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/Lobby/Lobby.h"

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
	ObjAdd(new Lobby());
}

void LobbyScene::Init(void)
{
	// オブジェクト全ての初期化処理
	for (ActorBase* obj : objects) { obj->Init(); }

	Camera::GetIns().ChangeModeFree(Deg2Rad(5.0f), 10.0f);
}

void LobbyScene::Update(void)
{
	// オブジェクト全ての更新処理
	for (ActorBase* obj : objects) { obj->Update(); }
}

void LobbyScene::Draw(void)
{
	// オブジェクト全ての描画処理
	for (ActorBase* obj : objects) { obj->Draw(); }
}

void LobbyScene::Release(void)
{
	// オブジェクト全ての解放処理
	for (ActorBase* obj : objects) {
		if (!obj) { continue; }
		obj->Release();
		delete obj;
		obj = nullptr;
	}
}
