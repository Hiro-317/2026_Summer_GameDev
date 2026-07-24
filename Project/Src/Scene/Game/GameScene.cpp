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

#include "Pause/GamePauseh.h"

#include "../Clear/ClearScene.h"

#include "../Common/GameDebugScene.h"

#include "../../Object/Character/Player/PlayerManager/PlayerManager.h"

#include "../../Object/Character/Boss/Tomato/TomatoBoss.h"
#include "../../Object/Character/Boss/Grape/GrapeBoss.h"
//#include "../../Object/Character/Boss/Banana/BananaBoss.h"

#include "../../Object/Stage/TomatoBoss/TomatoBossStage.h"
#include "../../Object/Stage/GrapeBoss/GrapeBossStage.h"

#include "../../Object/Common/DebugObject/BoxDebugObject.h"
#include "../../Object/Common/DebugObject/SphereDebugObject.h"
#include "../../Object/Common/DebugObject/CapsuleDebugObject.h"


int GameScene::hitStop = 0;

int GameScene::slow = 0;
int GameScene::slowInter = 0;

int GameScene::shake = 0;
ShakeKinds GameScene::shakeKinds = ShakeKinds::DIAG;
ShakeSize GameScene::shakeSize = ShakeSize::MEDIUM;

GameScene::GameScene():
	collision(nullptr),

	mainScreen(-1)
{
}

GameScene::~GameScene()
{
}

void GameScene::Load(void)
{
	Snd::GetIns().ChangeScene("Game");

	// 画面演出用
	mainScreen = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	// 当たり判定管理クラスを生成
	collision = new CollisionManager();
	
	// 初期化も含めたオブジェクト生成のラムダ関数
	auto ObjAdd = [&](ActorBase* newClass)->void {
		// 配列の末尾に追加
		objects.emplace_back(newClass);
		// 共通の読み込み処理
		objects.back()->Load();
		// そのオブジェクトが持つコライダーを管理クラスへ追加する
		collision->Add(objects.back()->GetCollider());
		};

	// オブジェクト生成（生成の順番がそのまま(更新/描画)順）
	//<例>ObjAdd(new Player());


	ObjAdd(new PlayerManager());

	// 接続されているプレイヤー数座標を取得する
	std::vector<const Vector3*> pos;
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		pos.emplace_back(&ObjSerch<PlayerManager>()->GetPlayerIns((MSG_SENDER_ID)id)->GetTrans().pos);
	}
	// 接続されているプレイヤー数生存判定を取得する
	std::vector<const bool*> live;
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		live.emplace_back(&ObjSerch<PlayerManager>()->GetPlayerIns((MSG_SENDER_ID)id)->GetIsDeath());
	}

	switch (SceneManager::GetIns().GetSelectBossType())
	{
	case BOSS_TYPE::Tomato:
		ObjAdd(new TomatoBoss(pos, live));
		ObjAdd(new TomatoBossStage());
		break;
	case BOSS_TYPE::Grape:
		ObjAdd(new GrapeBoss(pos, live));
		ObjAdd(new GrapeBossStage());
		break;
	default:
		break;
	}

	// 当たり判定のチャンク分け(初期化時)を行う
	collision->InitBuildChunks();

	// プレイヤーにボスの座標をわたす
	ObjSerch<PlayerManager>()->SetBossPos(&ObjSerch<BossBase>()->GetTrans().pos);
}

void GameScene::Init(void)
{
	// マウスカーソル
	Key::GetIns().SetMouseFixed(true);

	// オブジェクト全ての初期化処理
	for (ActorBase* obj : objects) { obj->Init(); }

	// カメラ設定
	Camera::GetIns().ChangeModeFollowRemote(
		&ObjSerch<PlayerManager>()->GetPlayerIns(Net::GetIns().GetSenderId())->GetTrans().pos,
		ObjSerch<PlayerManager>()->GetPlayerIns(Net::GetIns().GetSenderId())->GetInterestPos(),
		Vector3::YZonly(250, -550), Deg2Rad(4.0f)
	);
	focusFlg = false;

	SoundManager::GetIns().Play("Battle");
	//Camera::GetIns().ChangeModeFree(Deg2Rad(5.0f), 10.0f);
}

void GameScene::Update(void)
{
	// オブジェクト全ての受信処理
	for (ActorBase* obj : objects) { obj->ReceptionUpdate(); }

#pragma region 画面演出
	if (hitStop > 0) { hitStop--; return; }
	if (shake > 0) { shake--; }
	if (slow > 0) {
		slow--;
		if (slow % slowInter != 0) { return; }
	}
#pragma endregion

	// オブジェクト全ての更新処理
	for (ActorBase* obj : objects) { obj->Update(); }

	Vector3 pos = Vector3();

	// 注目するか
	if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_FOCUS).down) {
		focusFlg = !focusFlg;
		if (focusFlg) {
			Camera::GetIns().ChangeModeFollowAuto(
				ObjSerch<PlayerManager>()->GetPlayerIns(Net::GetIns().GetSenderId())->GetTrans(),
				&ObjSerch<BossBase>()->GetTrans().pos);
		}
		else {
			Camera::GetIns().ChangeModeFollowRemote(
				&ObjSerch<PlayerManager>()->GetPlayerIns(Net::GetIns().GetSenderId())->GetTrans().pos,
				ObjSerch<PlayerManager>()->GetPlayerIns(Net::GetIns().GetSenderId())->GetInterestPos(),
				Vector3::YZonly(250, -550), Deg2Rad(4.0f)
			);
		}
	}

	// 当たり判定
	collision->Check();

	// オブジェクト全ての送信処理
	for (ActorBase* obj : objects) { obj->SendUpdate(); }

#pragma region 遷移判定（ポーズも含む）
	// ポーズ判定
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

		Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::GamePause);

		SceneManager::GetIns().PushScene(std::make_shared<GamePause>(Net::GetIns().GetSenderId()));
		return;
	}
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>(MSG_SENDER_ID::None, true)) {
		SceneManager::GetIns().PushScene(std::make_shared<GamePause>(dataPtr->header.senderId));
		delete dataPtr;
	}
	
	// ゲームクリア判定
	if (ObjSerch<BossBase>()->GetIsDeath()) {

		switch (SceneManager::GetIns().GetSelectBossType())
		{
		case BOSS_TYPE::Tomato:
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::CLEAR, 90, 0xe33434, 0x000000);
			break;
		case BOSS_TYPE::Grape:
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::CLEAR, 90, 0x9338e8, 0x000000);
			break;
		default:
			break;
		}
		return;
	}

	// ゲームオーバー判定
	if (ObjSerch<PlayerManager>()->IsPlayerAllDeath()) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GAMEOVER, 90, 0xffffff, 0x000000);
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
	UpdateEffekseer3D();

#pragma endregion
}

void GameScene::Draw(void)
{
#pragma region 画面演出
	// 描画先を変更
	SetDrawScreen(mainScreen);

	// 画面リセット
	ClearDrawScreen();

	// カメラ適用
	Camera::GetIns().Apply();

	Effekseer_Sync3DSetting();

#pragma endregion

#pragma region 描画処理（メイン）

	// オブジェクト全ての描画処理
	for (ActorBase* obj : objects) { obj->Draw(); }

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	for (ActorBase* obj : objects) { obj->AlphaDraw(); }
	collision->DrawChunkGrid();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	Camera::GetIns().DrawDebug();

#pragma region エフェクト描画

	DrawEffekseer3D();

#pragma endregion
#pragma endregion

#pragma region 画面演出
	// 描画先を元に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 揺れの数値を算出
	Vector2I s = ShakePoint();

	// 揺れの数値分座標をずらして描画
	DrawGraph(s.x, s.y, mainScreen, true);
#pragma endregion

#pragma region UI描画（画面演出をかけないもの）
	// オブジェクト全てのUI描画処理
	for (ActorBase* obj : objects) { obj->UiDraw(); }
#pragma endregion
}

void GameScene::Release(void)
{
	Camera::GetIns().Release();

	// 当たり判定管理クラスの解放
	if (collision) {
		collision->Clear();
		delete collision;
		collision = nullptr;
	}

	// オブジェクト全ての解放処理
	for (ActorBase* obj : objects) {
		if (!obj) { continue; }
		obj->Release();
		delete obj;
		obj = nullptr;
	}

	// 画面演出用のメインスクリーンを解放
	DeleteGraph(mainScreen);
}

void GameScene::Shake(ShakeKinds kinds, ShakeSize size, int time)
{
	if ((abs(shake - time) > 10) || shake <= 0)shake = time;
	shakeKinds = kinds;
	shakeSize = size;
}
Vector2I GameScene::ShakePoint(void)
{
	Vector2I ret = {};

	if (shake > 0) {
		int size = shake / 5 % 2;
		size *= 2;
		size -= 1;
		switch (shakeKinds)
		{
		case GameScene::WID:ret.x = size;
			break;
		case GameScene::HIG:ret.y = size;
			break;
		case GameScene::DIAG:ret = size;
			break;
		case GameScene::ROUND:
			size = shake / 3 % 12; size++;
			ret = { (int)(((int)shakeSize * 1.5f) * cos(size * 30.0f)),(int)(((int)shakeSize * 1.5f) * sin(size * 30.0f)) };
			break;
		}

		if (shakeKinds != ShakeKinds::ROUND) { ret *= shakeSize; }

		DrawGraph(0, 0, mainScreen, true);
	}

	return ret;
}