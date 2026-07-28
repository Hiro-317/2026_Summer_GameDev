#include "SceneManager.h"

#include "../../Manager/Loading/Loading.h"
#include "../../Manager/Effect/EffectManager.h"

#include "../Title/TitleScene.h"
#include "../Lobby/LobbyScene.h"
#include "../Lobby/MultiLobbyScene.h"
#include "../Game/GameScene.h"
#include "../Clear/ClearScene.h"
#include"../GameOver/GameOver.h"

#include "../Common/Fade/FadeInScene.h"
#include "../Common/Fade/FadeOutScene.h"

SceneManager* SceneManager::ins = nullptr;

// 初期化
void SceneManager::Init(void)
{
	// ロード画面生成
	Loading::GetInstance()->CreateInstance();
	Loading::GetInstance()->Init();
	Loading::GetInstance()->Load();

	Init3D();

	// 最初はタイトル画面から
	ChangeScene(SCENE_ID::TITLE);
}


// 更新
void SceneManager::Update(void)
{
	// シーンがなければ終了
	if (scenes.empty()) { return; }

	// ロード中
	if (Loading::GetInstance()->IsLoading())
	{
		// ロード更新
		Loading::GetInstance()->Update();

		// ロードの更新が終了していたら
		if (Loading::GetInstance()->IsLoading() == false)
		{
			// ロード後の初期化
			for (auto& scene : scenes) { scene->Init(); }
		}
		
	}		
	// 通常の更新処理
	else
	{
		// 現在のシーンの更新
		scenes.back()->Update();
	}
}

// 描画
void SceneManager::Draw(void)
{
	// ロード中ならロード画面を描画
	if (Loading::GetInstance()->IsLoading()) {
		// ロードの描画
		Loading::GetInstance()->Draw();
	}
	// 通常の更新
	else {
		// 積まれているもの全てを描画する
		for (auto& scene : scenes)
		{
			scene->Draw();
		}
	}
}

// 解放
void SceneManager::Release(void)
{
	//全てのシーンの解放・削除
	for (auto& scene : scenes) { scene->Release(); }
	scenes.clear();

	DeleteLightHandleAll();

	// ロード画面の削除
	Loading::GetInstance()->Release();
	Loading::GetInstance()->DeleteInstance();
}

// 状態遷移関数
void SceneManager::ChangeScene(std::shared_ptr<SceneBase>scene)
{
	//エフェクトの開放
	EffectManager::GetIns()->StopEffectAll();
	// シーンが空か？
	if (scenes.empty()) {
		//空なので新しく入れる
		scenes.push_back(scene);
	}
	else {
		//末尾のものを新しい物に入れ替える
		scenes.back()->Release();
		scenes.back() = scene;
	}

	// 読み込み(非同期)
	Loading::GetInstance()->StartAsyncLoad();
	scenes.back()->Load();
	Loading::GetInstance()->EndAsyncLoad();
}

void SceneManager::ChangeScene(SCENE_ID scene)
{
	switch (scene)
	{
	case SCENE_ID::TITLE:
		ChangeScene(std::make_shared<TitleScene>());
		break;
	case SCENE_ID::LOBBY:
		ChangeScene(std::make_shared<LobbyScene>());
		break;
	case SCENE_ID::MULTI_LOBBY:
		ChangeScene(std::make_shared<MultiLobbyScene>());
		break;
	case SCENE_ID::GAME:
		ChangeScene(std::make_shared<GameScene>());
		break;
	case SCENE_ID::CLEAR:
		ChangeScene(std::make_shared<ClearScene>());
		break;
	case SCENE_ID::GAMEOVER:
		ChangeScene(std::make_shared<GameOver>());
		break;
	default:
		break;
	}
}

void SceneManager::ChangeSceneFade(std::shared_ptr<SceneBase> scene, unsigned short FADE_TIME, unsigned int FADE_COLOR, unsigned int FADE_OUT_COLOR)
{
	PushScene(std::make_shared<FadeInScene>(scene, FADE_TIME, FADE_COLOR, FADE_OUT_COLOR, true));
}

void SceneManager::ChangeSceneFade(SCENE_ID scene, unsigned short FADE_TIME, unsigned int FADE_COLOR, unsigned int FADE_OUT_COLOR)
{
	switch (scene)
	{
	case SCENE_ID::TITLE:
		ChangeSceneFade(std::make_shared<TitleScene>(), FADE_TIME, FADE_COLOR, FADE_OUT_COLOR);
		break;
	case SCENE_ID::LOBBY:
		ChangeSceneFade(std::make_shared<LobbyScene>(), FADE_TIME, FADE_COLOR, FADE_OUT_COLOR);
		break;
	case SCENE_ID::MULTI_LOBBY:
		ChangeSceneFade(std::make_shared<MultiLobbyScene>(), FADE_TIME, FADE_COLOR, FADE_OUT_COLOR);
		break;
	case SCENE_ID::GAME:
		ChangeSceneFade(std::make_shared<GameScene>(), FADE_TIME, FADE_COLOR, FADE_OUT_COLOR);
		break;
	case SCENE_ID::CLEAR:
		ChangeSceneFade(std::make_shared<ClearScene>(), FADE_TIME, FADE_COLOR, FADE_OUT_COLOR);
		break;
	case SCENE_ID::GAMEOVER:
		ChangeSceneFade(std::make_shared<GameOver>(), FADE_TIME, FADE_COLOR, FADE_OUT_COLOR);
		break;
	default:
		break;
	}
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	//新しく積むのでもともと入っている奴はまだ削除されない
	scenes.push_back(scene);
	scenes.back()->Load();
	scenes.back()->Init();
}

void SceneManager::PushScene(SCENE_ID scene)
{
	switch (scene)
	{
	case SCENE_ID::TITLE:
		PushScene(std::make_shared<TitleScene>());
		break;
	case SCENE_ID::LOBBY:
		PushScene(std::make_shared<LobbyScene>());
		break;
	case SCENE_ID::MULTI_LOBBY:
		PushScene(std::make_shared<MultiLobbyScene>());
		break;
	case SCENE_ID::GAME:
		PushScene(std::make_shared<GameScene>());
		break;
	case SCENE_ID::CLEAR:
		PushScene(std::make_shared<ClearScene>());
		break;
	case SCENE_ID::GAMEOVER:
		PushScene(std::make_shared<GameOver>());
		break;
	default:
		break;
	}
}

void SceneManager::PopScene(void)
{
	//積んであるものを消して、もともとあったものを末尾にする
	if (scenes.size() > 0) 
	{
		scenes.back()->Release();
		scenes.pop_back();
	}
}

void SceneManager::JumpScene(std::shared_ptr<SceneBase> scene)
{
	// 全て解放
	for (auto& s : scenes) { s->Release(); }
	scenes.clear();

	// 新しく積む
	ChangeScene(scene);
}

void SceneManager::JumpScene(SCENE_ID scene)
{
	switch (scene)
	{
	case SCENE_ID::TITLE:
		JumpScene(std::make_shared<TitleScene>());
		break;
	case SCENE_ID::LOBBY:
		JumpScene(std::make_shared<LobbyScene>());
		break;
	case SCENE_ID::MULTI_LOBBY:
		JumpScene(std::make_shared<MultiLobbyScene>());
		break;
	case SCENE_ID::GAME:
		JumpScene(std::make_shared<GameScene>());
		break;
	case SCENE_ID::CLEAR:
		JumpScene(std::make_shared<ClearScene>());
		break;
	case SCENE_ID::GAMEOVER:
		JumpScene(std::make_shared<GameOver>());
		break;
	default:
		break;
	}
}

void SceneManager::JumpSceneFade(std::shared_ptr<SceneBase> scene, unsigned short FADE_TIME, unsigned int FADE_COLOR)
{
	PushScene(std::make_shared<FadeInScene>(scene, FADE_TIME, FADE_COLOR, FADE_COLOR, false));
}

void SceneManager::JumpSceneFade(SCENE_ID scene, unsigned short FADE_TIME, unsigned int FADE_COLOR)
{
	switch (scene)
	{
	case SCENE_ID::TITLE:
		JumpSceneFade(std::make_shared<TitleScene>());
		break;
	case SCENE_ID::LOBBY:
		JumpSceneFade(std::make_shared<LobbyScene>());
		break;
	case SCENE_ID::MULTI_LOBBY:
		JumpSceneFade(std::make_shared<MultiLobbyScene>());
		break;
	case SCENE_ID::GAME:
		JumpSceneFade(std::make_shared<GameScene>());
		break;
	case SCENE_ID::CLEAR:
		JumpSceneFade(std::make_shared<ClearScene>());
		break;
	case SCENE_ID::GAMEOVER:
		JumpSceneFade(std::make_shared<GameOver>());
		break;
	default:
		break;
	}
}

void SceneManager::AnyPopAndChangeScene(char popNum, std::shared_ptr<SceneBase> scene)
{
	for (char i = 0; i < popNum; i++) { PopScene(); }
	ChangeScene(scene);
}

void SceneManager::Init3D(void)
{
#pragma region 基本描画設定

	SetBackgroundColor(0, 0, 0);
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);
	SetTextureAddressMode(DX_TEXADDRESS_WRAP);
	SetDrawBright(255, 255, 255);

#pragma endregion

#pragma region フォグ設定

	SetFogEnable(true);
	SetFogColor(200, 200, 200);
	SetFogStartEnd(5000.0f, 10000.0f);

#pragma endregion

#pragma region ライティング全体設定

	SetUseLighting(true);
	SetUseSpecular(false);
	SetGlobalAmbientLight(GetColorF(0.38f, 0.38f, 0.38f, 1.0f));

#pragma endregion

#pragma region メインライト設定

	VECTOR mainDirection = VNorm(VGet(0.0f, 0.35f, 1.0f));
	ChangeLightTypeDir(mainDirection);
	SetLightDirection(mainDirection);
	SetLightEnable(true);
	SetLightDifColor(GetColorF(0.62f, 0.62f, 0.62f, 1.0f));
	SetLightAmbColor(GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	SetLightSpcColor(GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

#pragma endregion

#pragma region 補助ライト設定

	fillLight = CreateDirLightHandle(VNorm(VGet(0.05f, 0.15f, 1.0f)));

	if (fillLight >= 0) {
		SetLightDifColorHandle(fillLight, GetColorF(0.20f, 0.20f, 0.20f, 1.0f));
		SetLightAmbColorHandle(fillLight, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		SetLightSpcColorHandle(fillLight, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		SetLightEnableHandle(fillLight, true);
	}

#pragma endregion

#pragma region リムライト設定

	rimLight = CreateDirLightHandle(VNorm(VGet(0.0f, 0.10f, -1.0f)));

	if (rimLight >= 0) {
		SetLightDifColorHandle(rimLight, GetColorF(0.13f, 0.13f, 0.13f, 1.0f));
		SetLightAmbColorHandle(rimLight, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		SetLightSpcColorHandle(rimLight, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		SetLightEnableHandle(rimLight, true);
	}

#pragma endregion

#pragma region マテリアル設定

	MATERIALPARAM material{};
	material.Diffuse = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = GetColorF(0.6f, 0.6f, 0.6f, 1.0f);
	material.Specular = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	material.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	material.Power = 0.0f;
	SetMaterialParam(material);

#pragma endregion
}
