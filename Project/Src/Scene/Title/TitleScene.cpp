#include"TitleScene.h"

#include"../../Utility/Utility.h"

#include"../../Application/Application.h"

#include"../../Manager/Net/NetWorkManager.h"
#include"../../Manager/Input/KeyManager.h"
#include"../../Manager/Camera/Camera.h"
#include"../../Manager/Sound/SoundManager.h"
#include"../../Manager/Font/FontManager.h"

#include"../SceneManager/SceneManager.h"

#include"End/EndScene.h"

TitleScene::TitleScene():
	image(-1)
{
}

void TitleScene::Load(void)
{
	// 音声のシーン切り替え
	Snd::GetIns().ChangeScene("Title");

	Key::GetIns().SetMouseFixed(false);

	image = LoadGraph("Data/Image/Title/Title.png");
}
void TitleScene::Init(void)
{
	// カメラの初期化
	Camera::GetIns().ChangeModeFixedPoint(Vector3(), Vector3());

	Net::GetIns().Disconnection();
}
void TitleScene::Update(void)
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
		//Net::GetIns().StartHost();
		//SceneManager::GetIns().SetSelectCharaType(Net::HOST_SENDER_ID, CHARA_TYPE::Orange);
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::LOBBY);
		return;
	}

}
void TitleScene::Draw(void)
{
	DrawGraph(0, 0, image, true);
}
void TitleScene::Release(void)
{
	DeleteGraph(image);
}
