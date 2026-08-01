#include "GameScene.h"

#include <cmath>

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"

#include "../../Manager/Net/NetWorkManager.h"
#include "../../Manager/Camera/Camera.h"
#include "../../Manager/Input/KeyManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"
#include "../../Manager/Effect/EffectManager.h"

#include "../../scene/SceneManager/SceneManager.h"

#include "../ObjectUseDefine.h"

#include "Pause/GamePauseh.h"

#include "../Common/GameDebugScene.h"

#include "../../Object/Character/Player/PlayerManager/PlayerManager.h"

#include "../../Object/Character/Boss/Tomato/TomatoBoss.h"
#include "../../Object/Character/Boss/Grape/GrapeBoss.h"
#include "../../Object/Character/Boss/Banana/BananaBoss.h"

#include "../../Object/Stage/TomatoBoss/TomatoBossStage.h"
#include "../../Object/Stage/GrapeBoss/GrapeBossStage.h"

#include "../../Object/Common/DebugObject/BoxDebugObject.h"
#include "../../Object/Common/DebugObject/SphereDebugObject.h"
#include "../../Object/Common/DebugObject/CapsuleDebugObject.h"


GameScene::GameScene() :
	SceneBase(),
	focusFlg(false)
{
}

void GameScene::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("Game");

	CreateObject<PlayerManager>();

	// 接続されているプレイヤー数座標を取得する
	std::vector<const Vector3*> pos;
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		pos.emplace_back(&ObjSerch<PlayerManager>(objects)->GetPlayerIns((MSG_SENDER_ID)id)->GetTrans().pos);
	}
	// 接続されているプレイヤー数生存判定を取得する
	std::vector<const bool*> live;
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		live.emplace_back(&ObjSerch<PlayerManager>(objects)->GetPlayerIns((MSG_SENDER_ID)id)->GetIsDeath());
	}

	switch (SceneManager::GetIns().GetSelectBossType())
	{
	case BOSS_TYPE::Tomato:
		CreateObject<TomatoBoss>(pos, live);
		CreateObject<TomatoBossStage>();
		break;
	case BOSS_TYPE::Grape:
		CreateObject<GrapeBoss>(pos, live);
		CreateObject<GrapeBossStage>();
		break;
	default:
		break;
	}

	// プレイヤーにボスの座標をわたす
	ObjSerch<PlayerManager>(objects)->SetBossPos(&ObjSerch<BossBase>(objects)->GetTrans().pos);
}

void GameScene::SubPostInit(void)
{
	// マウスカーソル
	Key::GetIns().SetMouseFixed(true);

	// カメラ設定
	camera->ChangeModeFollowRemote(
		&ObjSerch<PlayerManager>(objects)->GetPlayerIns(Net::GetIns().GetSenderId())->GetTrans().pos,
		ObjSerch<PlayerManager>(objects)->GetPlayerIns(Net::GetIns().GetSenderId())->GetInterestPos(),
		Vector3::YZonly(250, -550), Deg2Rad(4.0f)
	);
	focusFlg = false;

	SoundManager::GetIns().Play("Battle");
}

void GameScene::SubPostUpdate(void)
{
	Vector3 pos = Vector3();

	// 注目するか
	if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_FOCUS).down) {
		focusFlg = !focusFlg;
		if (focusFlg) {
			camera->ChangeModeFollowAuto(
				ObjSerch<PlayerManager>(objects)->GetPlayerIns(Net::GetIns().GetSenderId())->GetTrans(),
				&ObjSerch<BossBase>(objects)->GetTrans().pos);
		}
		else {
			camera->ChangeModeFollowRemote(
				&ObjSerch<PlayerManager>(objects)->GetPlayerIns(Net::GetIns().GetSenderId())->GetTrans().pos,
				ObjSerch<PlayerManager>(objects)->GetPlayerIns(Net::GetIns().GetSenderId())->GetInterestPos(),
				Vector3::YZonly(250, -550), Deg2Rad(4.0f)
			);
		}
	}

#pragma region 遷移判定（ポーズも含む）
	// ポーズ判定
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

		Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::GamePause);

		SceneManager::GetIns().PushScene(std::make_unique<GamePause>(Net::GetIns().GetSenderId()));
		return;
	}
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>(MSG_SENDER_ID::None, true)) {
		SceneManager::GetIns().PushScene(std::make_unique<GamePause>(dataPtr->header.senderId));
		delete dataPtr;
	}
	
	// ゲームクリア判定
	if (ObjSerch<BossBase>(objects)->GetIsDeath()) {

		switch (SceneManager::GetIns().GetSelectBossType())
		{
		case BOSS_TYPE::Tomato:
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::CLEAR, FADE_TYPE::DEFAULT, 90, 0xe33434, 0x000000);
			break;
		case BOSS_TYPE::Grape:
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::CLEAR, FADE_TYPE::DEFAULT, 90, 0x9338e8, 0x000000);
			break;
		default:
			break;
		}
		return;
	}

	// ゲームオーバー判定
	if (ObjSerch<PlayerManager>(objects)->IsPlayerAllDeath()) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GAMEOVER, FADE_TYPE::DEFAULT, 90, 0xffffff, 0x000000);
		return;
	}
	
#if _DEBUG
	// シーンを再読み込み
	if (Key::GetIns().GetInfo(KEY_TYPE::DEBUG_RELOAD).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GAME);
		return;
	}
#endif // _DEBUG

	EffectManager::GetIns()->Update();
	EffectManager::GetIns()->ReceptionUpdate();
	UpdateEffekseer3D();

#pragma endregion
}