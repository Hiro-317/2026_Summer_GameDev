#include "ClearScene.h"

#include"../../Utility/Utility.h"

#include"../../Application/Application.h"

#include"../../Manager/Input/KeyManager.h"
#include"../../Manager/Sound/SoundManager.h"
#include"../../Manager/Font/FontManager.h"

#include"../SceneManager/SceneManager.h"

ClearScene::ClearScene() : SceneBase() {}

void ClearScene::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("Clear");

	Key::GetIns().SetMouseFixed(false);

	image = LoadGraph("Data/Image/Clear/GameClearImage.png");
}

void ClearScene::SubPostUpdate(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down ||
		Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::TITLE);
		Snd::GetIns().Play("SystemButton");
		return;
	}
}

void ClearScene::SubPostDraw(void)
{
	DrawExtendGraph(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, image, true);
}

void ClearScene::SubPreRelease(void)
{
	DeleteGraph(image);
}