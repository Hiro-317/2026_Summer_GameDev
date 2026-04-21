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
#include"../../Object/CharaSelect/Tomato/TomatoSelect.h"
#include"../../Object/CharaSelect/Tanaka/TanakaSelect.h"

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
	objects.push_back(new TomatoSelect());
	objects.push_back(new TanakaSelect());

	for (CharaSelectObj* i : objects) {

		i->Load();
	}
	tempNowChara = static_cast<int>(CharaSelectObj::SELECT_PLAYER::NON) + 1;
}
void CharaSelect::Init(void)
{
	// カメラの初期化
	Camera::GetIns().ChangeModeFixedPoint(Vector3(0, 150, -350), Vector3());
}
void CharaSelect::Update(void)
{
	// キャラクターの更新
	tempNowChara += objects.at(tempNowChara)->GetNowChara();

	if (tempNowChara <= static_cast<int>(CharaSelectObj::SELECT_PLAYER::NON)) {
		tempNowChara = 0;
	}
	if (tempNowChara >= static_cast<int>(CharaSelectObj::SELECT_PLAYER::MAX)) {
		tempNowChara = static_cast<int>(CharaSelectObj::SELECT_PLAYER::MAX) - 1;
	}

	objects.at(tempNowChara)->Update();

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
	
	// キャラクターの描画
	objects.at(tempNowChara)->Draw();

	DrawFormatStringToHandle(0, 0, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_64), "キャラクターシーン");
}
void CharaSelect::Release(void)
{
	// キャラクターの解放
	for (ActorBase* i : objects) {

		i->Release();
	}
	objects.clear();
}
