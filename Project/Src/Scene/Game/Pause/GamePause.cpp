#include "GamePauseh.h"

#include "../../../Utility/Utility.h"

#include "../../../Application/Application.h"
#include "../../SceneManager/SceneManager.h"
#include "../../../Manager/Input/KeyManager.h"
#include "../../../Manager/Sound/SoundManager.h"
#include "../../../Manager/Net/NetWorkManager.h"

GamePause::GamePause(MSG_SENDER_ID operatorSenderId) :
	img(),
	nowSelect(SELECT::YES),

	operatorSenderId(operatorSenderId),
	isOperator(operatorSenderId == MSG_SENDER_ID::None ? true : operatorSenderId == Net::GetIns().GetSenderId())
{
}

GamePause::~GamePause()
{
}

void GamePause::Load(void)
{
	img[(int)SELECT::YES] = LoadImg("Data/Image/Title/End/Yes.png");
	img[(int)SELECT::NO] = LoadImg("Data/Image/Title/End/No.png");
}

void GamePause::Init(void)
{
	nowSelect = SELECT::YES;
	SoundManager::GetIns().AllStop();
	Key::GetIns().SetMouseFixed(false);
}

void GamePause::Update(void)
{
	// 自分がポーズ画面を開いたとき
	if (isOperator) {

		if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

			Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::GamePauseEnd);

			SoundManager::GetIns().PausePlay();
			Snd::GetIns().Play("SystemButton");
			SceneManager::GetIns().PopScene();
			return;
		}

		switch (nowSelect) {

		case GamePause::SELECT::YES:
			if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {

				Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::GamePauseChoicesSwitch);

				nowSelect = GamePause::SELECT::NO;
				Snd::GetIns().Play("SystemSelect");
			}

			if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

				Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::ChangeSceneLobby);

				Snd::GetIns().Play("SystemButton");
				SceneManager::GetIns().JumpSceneFade(SCENE_ID::LOBBY);
				return;
			}

			break;

		case GamePause::SELECT::NO:

			if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {

				Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::GamePauseChoicesSwitch);

				nowSelect = GamePause::SELECT::YES;
				Snd::GetIns().Play("SystemSelect");
			}

			if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

				Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::GamePauseEnd);

				Snd::GetIns().PausePlay();
				Snd::GetIns().Play("SystemButton");
				SceneManager::GetIns().PopScene();
				Key::GetIns().SetMouseFixed(true);
				return;
			}

			break;
		}

	}
	// 自分以外がポーズ画面を開いたとき(受信処理のみ)
	else {

		while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>(MSG_SENDER_ID::None, true)) {

			bool isReturn = false;

			switch (dataPtr->inform) {
			case MsgDataSystemInform::INFORM_TYPE::None: { break; }

			case MsgDataSystemInform::INFORM_TYPE::ChangeSceneLobby: {
				Snd::GetIns().Play("SystemButton");
				SceneManager::GetIns().JumpSceneFade(SCENE_ID::LOBBY);
				isReturn = true;
				break;
			}
			case MsgDataSystemInform::INFORM_TYPE::GamePauseEnd: {
				Snd::GetIns().PausePlay();
				Snd::GetIns().Play("SystemButton");
				SceneManager::GetIns().PopScene();
				Key::GetIns().SetMouseFixed(true);
				isReturn = true;
				break;
			}
			case MsgDataSystemInform::INFORM_TYPE::GamePauseChoicesSwitch: {
				Snd::GetIns().Play("SystemSelect");
				if (nowSelect == SELECT::YES) { nowSelect = SELECT::NO; }
				else if (nowSelect == SELECT::NO) { nowSelect = SELECT::YES; }

				break;
			}
			default: { break; }
			}

			delete dataPtr;

			if (isReturn) { return; }
		}

	}
}

void GamePause::Draw(void)
{
	int xx = Application::SCREEN_SIZE_X;
	int yy = Application::SCREEN_SIZE_Y;
	int x = xx / 2;
	int y = yy / 2;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, xx, yy, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawRotaGraph(x, y, 1, 0, img[(int)nowSelect], true);
}

void GamePause::Release(void)
{
	for (auto& id : img) { DeleteGraph(id); }

}