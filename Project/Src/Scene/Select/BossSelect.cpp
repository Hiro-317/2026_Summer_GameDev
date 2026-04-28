#include"BossSelect.h"

#include<DxLib.h>
#include"../../Utility/Utility.h"

#include"../../Application/Application.h"

#include"../../Manager/Input/KeyManager.h"
#include"../../Manager/Camera/Camera.h"
#include"../../Manager/Sound/SoundManager.h"
#include"../../Manager/Font/FontManager.h"

#include"../SceneManager/SceneManager.h"

#include"../Title/End/EndScene.h"

BossSelect::BossSelect()
{
}

void BossSelect::Load(void)
{
	// 音声のシーン切り替え
	Snd::GetIns().ChangeScene("Title");

	Key::GetIns().SetMouseFixed(false);
}
void BossSelect::Init(void)
{
	// カメラの初期化
	Camera::GetIns().ChangeModeFixedPoint(Vector3(), Vector3());
}
void BossSelect::Update(void)
{
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
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GAME);
		return;
	}
}
void BossSelect::Draw(void)
{
	DrawFormatStringToHandle(0, 0, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_64), "ボスシーン");
}
void BossSelect::Release(void)
{
}
