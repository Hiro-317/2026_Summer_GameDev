#include "ClearScene.h"

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"

#include "../../Manager/Input/KeyManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"
#include "../../Manager/Net/NetWorkManager.h"

#include "../SceneManager/SceneManager.h"

ClearScene::ClearScene() : SceneBase() {}

void ClearScene::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("Clear");

	image = LoadGraph("Data/Image/Clear/GameClearImage.png");
}

void ClearScene::SubPostUpdate(void)
{
	if (Net::GetIns().IsHost()) {
		if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down ||
			Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

			if (Net::GetIns().GetState() == Net::NetState::None) {
				SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Lobby);
			}
			else {
				Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::ChangeSceneLobby);
				Net::GetIns().ResumptionReceptionToHosting();
				SceneManager::GetIns().ChangeSceneFade(SCENE_ID::MultiLobby);
			}
			Snd::GetIns().Play("SystemButton");
		}
	}
	else {
		while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>()) {

			if (dataPtr->inform == MsgDataSystemInform::INFORM_TYPE::ChangeSceneLobby) {
				SceneManager::GetIns().ChangeSceneFade(SCENE_ID::MultiLobby);
				Snd::GetIns().Play("SystemButton");
			}

			delete dataPtr;
		}
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