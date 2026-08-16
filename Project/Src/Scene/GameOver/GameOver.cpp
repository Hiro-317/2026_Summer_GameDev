#include "GameOver.h"

#include"../../Utility/Utility.h"

#include"../../Application/Application.h"

#include"../../Manager/Input/KeyManager.h"
#include"../../Manager/Sound/SoundManager.h"
#include"../../Manager/Font/FontManager.h"

#include"../SceneManager/SceneManager.h"

GameOver::GameOver() : SceneBase() {}

void GameOver::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("GameOver");

	image = LoadGraph("Data/Image/GameOver/GameOverImage.png");
}

void GameOver::SubPostUpdate(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down ||
		Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Title);
		Snd::GetIns().Play("SystemButton");
		return;
	}
}

void GameOver::SubPostDraw(void)
{
	DrawExtendGraph(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, image, true);
}

void GameOver::SubPreRelease(void)
{
	DeleteGraph(image);
}