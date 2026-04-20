#include"CharaSelect.h"

#include<DxLib.h>
#include"../../Utility/Utility.h"

#include"../../Application/Application.h"

#include"../../Manager/Input/KeyManager.h"
#include"../../Manager/Camera/Camera.h"
#include"../../Manager/Sound/SoundManager.h"
#include"../../Manager/Font/FontManager.h"

#include"../SceneManager/SceneManager.h"

#include"../Title/End/EndScene.h"
#include"../../Object/CharaSelect/Orange/OrangeSelect.h"

CharaSelect::CharaSelect()
{
}

void CharaSelect::Load(void)
{
	// 音声のシーン切り替え
	Snd::GetIns().ChangeScene("Title");

	Key::GetIns().SetMouceFixed(false);

	// キャラクターのロード
	objects.push_back(new OrangeSelect());
	objects.back()->Load();
}
void CharaSelect::Init(void)
{
	// カメラの初期化
	Camera::GetIns().ChangeModeFixedPoint(Vector3(0, 150, -350), Vector3());

	// キャラクターの初期化
	objects.back()->Init();
}
void CharaSelect::Update(void)
{
	objects.back()->Update();

	// ゲーム終了処理
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {
		Snd::GetIns().Pause();
		Snd::GetIns().Play("SystemSelect");
		SceneManager::GetIns().PushScene(std::make_shared<EndScene>());
		return;
	}

	// シーン進行処理
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
		Snd::GetIns().Play("SystemButton");
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::BOSS);
		return;
	}
}
void CharaSelect::Draw(void)
{
#pragma region 画面演出

	//SetDrawScreen(DX_SCREEN_BACK);

	//// 画面リセット
	//ClearDrawScreen();

	//// カメラ適用
	//Camera::GetIns().Apply();
	
	objects.back()->Draw();

	DrawFormatStringToHandle(0, 0, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_64), "キャラクターシーン");
}
void CharaSelect::Release(void)
{
	objects.back()->Release();
	objects.clear();
}
