#include "BossSelectScene.h"

#include "../../Manager/Net/NetWorkManager.h"
#include "../../Manager/Input/InputManager.h"

#include "../../Manager/Camera/CameraEvent/LookAtCameraEvent/LookAtCameraEvent.h"

#include "../../Scene/SceneManager.h"
#include "../ActorUseDefine.h"

#include "BossConfirm/BossConfirmScene.h"

#include "../../Manager/Camera/FollowRemote/FollowRemoteCamera.h"
#include "../../Manager/Camera/Free/FreeCamera.h"

#include "../../Object/Common/SkyBox/SkyBox.h"
#include "../../Object/BossSelect/BossSelectStage.h"
#include "../../Object/BossSelect/SkyShip/BossSelectSkyShip.h"

#include "../../Object/BossSelect/Spot/BossSelectSpotManager.h"

BossSelectScene::BossSelectScene() :
	SceneBase(),

	boardImage(-1),
	operatorGuideImage()
{
}

void BossSelectScene::SubPostLoad(void)
{
	AddActor(new SkyDome(200.0f));
	AddActor(new BossSelectStage());
	AddActor(new BossSelectSpotManager());
	AddActor(new BossSelectSkyShip());

	boardImage = LoadGraph("Data/Image/BossSelect/StageSelectBoard.png");

	if (Net::GetIns().IsHost()) {
		operatorGuideImage[(int)true] = LoadGraph("Data/Image/BossSelect/BossSelectOperatorGuideController.png");
		operatorGuideImage[(int)false] = LoadGraph("Data/Image/BossSelect/BossSelectOperatorGuideKeyboard.png");
	}
}

void BossSelectScene::SubPostInit(void)
{
	if (Net::GetIns().GetState() == Net::NetState::None) { return; }

	// 選択キャラの受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataCharaSelect>(MSG_SENDER_ID::None,true)) {

		// 受け取ったキャラタイプを保存する
		SceneManager::GetIns().SetSelectCharaType(dataPtr->header.senderId, (CHARA_TYPE)dataPtr->charaType);

		delete dataPtr;
	}
}

void BossSelectScene::SubPostUpdate(void)
{
	if (Net::GetIns().IsHost() && Input::GetIns().GetInfo(KEY_TYPE::Pause).down) {

		if (Net::GetIns().GetState() == Net::NetState::None) {
			// シーン遷移
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Lobby);
		}
		else {
			// シーン遷移を通知
			Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::ChangeSceneLobby);

			// 受付再開
			Net::GetIns().ResumptionReceptionToHosting();

			// シーン遷移
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::MultiLobby);
		}
	}

	if (Net::GetIns().GetState() == Net::NetState::None) { return; }

	// 切断 の受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataConnectInform>()) {

		if (dataPtr->inform == MsgDataConnectInform::INFORM_TYPE::Disconnect) {
			Net::GetIns().Disconnection();
			SceneManager::GetIns().JumpSceneFade(SCENE_ID::Lobby);
		}

		delete dataPtr;
	}

	// システム通知の受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>()) {

		// シーン遷移の受信
		if (dataPtr->inform == MsgDataSystemInform::INFORM_TYPE::PushSceneBossConfirm) {
			SceneManager::GetIns().PushScene(std::make_unique<BossConfirmScene>());
		}
		if (dataPtr->inform == MsgDataSystemInform::INFORM_TYPE::ChangeSceneLobby) {
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::MultiLobby);
		}

		delete dataPtr;
	}

	// 選択キャラの受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataCharaSelect>(MSG_SENDER_ID::None, true)) {

		// 受け取ったキャラタイプを保存する
		SceneManager::GetIns().SetSelectCharaType(dataPtr->header.senderId, (CHARA_TYPE)dataPtr->charaType);

		delete dataPtr;
	}

	// 選択ボスの受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataBossSelect>()) {

		// 受け取ったボスタイプを保存する
		SceneManager::GetIns().SetSelectBossType((BOSS_TYPE)dataPtr->bossType);

		delete dataPtr;
	}

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataCameraEvent>()) {
		camera->EndEvent();
		delete dataPtr;
	}
}

void BossSelectScene::SubUiDraw(void)
{
	DrawGraph(0, 0, boardImage, true);

	if (Net::GetIns().IsHost()) {
		DrawRotaGraph3(0, App::SCREEN_SIZE_Y, 0, 300, 0.8, 0.8, 0, operatorGuideImage[(int)Input::GetIns().LastInputKinds()], true);
	}
}

void BossSelectScene::SubPostRelease(void)
{
	if (Net::GetIns().IsHost()) {
		DeleteGraph(operatorGuideImage[(int)true]);
		DeleteGraph(operatorGuideImage[(int)false]);
	}

	DeleteGraph(boardImage);
}

void BossSelectScene::CreateCamera(void)
{
	// 追従カメラ生成
	camera = new FollowRemoteCamera(
		&ActorSerch<BossSelectSkyShip>(actors)->GetTrans().pos,
		Vector3::YZonly(500.0f, -500.0f),
		Vector3::Zonly(500.0f),
		0.0f
	);

	camera->StartEvent(new LookAtCameraEvent(ActorSerch<BossSelectSkyShip>(actors)->GetTrans().pos, Vector3::Zonly(500.0f), INT_MAX));
}
