#include "GameOver.h"

#include"../../Utility/Utility.h"

#include"../../Application/Application.h"

#include"../../Manager/Input/KeyManager.h"
#include"../../Manager/Sound/SoundManager.h"
#include"../../Manager/Font/FontManager.h"

#include"../SceneManager/SceneManager.h"

GameOver::GameOver()
{
}

GameOver::~GameOver()
{
}

void GameOver::Load(void)
{
	Snd::GetIns().ChangeScene("GameOver");

	Key::GetIns().SetMouseFixed(false);
}

void GameOver::Init(void)
{
}

void GameOver::Update(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down ||
		Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::TITLE);
		Snd::GetIns().Play("SystemButton");
		return;
	}
}

void GameOver::Draw(void)
{
	DrawFormatStringToHandle(0, 0, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_64), "ゲームオーバーシーン");
}

void GameOver::Release(void)
{
}