#include "TomatoBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Font/FontManager.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"

#include "../../../../Scene/Game/GameScene.h"

#include "State/Idle/TomatoBossIdleState.h"
#include "State/Headbutt/TomatoBossHeadbuttState.h"
#include "State/Headbutt/TomatoHeadbuttCollOperator.h"
#include "State/Move/TomatoBossMoveState.h"
#include "State/Tackle/TomatoBossTackleState.h"
#include "State/Tackle/TomatoTackleCollOperator.h"
#include "State/Stamp/TomatoStampState.h"
#include "State/Stamp/TomatoStampCollOperator.h"

#include "../../../../Manager/Effect/EffectManager.h"

#include "../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../UI/HitUI/HitUI.h"

TomatoBoss::TomatoBoss(const std::vector<const Vector3*> playerPos) :
	CharacterBase(
		"TomatoBossParameter",
		"TomatoBossHP",
		"TomatoBossAttackPower",
		"TomatoBossDefensePower",
		"TomatoBossMoveSpeed",
		"Data/Parameter/Character/Boss/Tomato/"),
	subObjArray(),
	playerPos(playerPos)
{
	collParam = new ParameterLoad("Data/Parameter/AttackRange/");

	coolTime = 120;

	this->operatorSenderId = Net::HOST_SENDER_ID;
	isOwnOperator = true;

	rockHit = false;

	targetNum = 0;

	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		damaged.emplace_back(0);
	}

	mostDamaged = 0;
}

void TomatoBoss::CharacterLoad(void)
{
#pragma region モデル

	// モデルを読み込む
	trans.Load("Charactor/Tomato/Tomato");

	// モデルのスケールを設定
	trans.scale = MODEL_SCALE;

	// モデルの中心点のズレ
	trans.centerDiff = MODEL_CENTER_DIFF;

	// 角度のズレ(ローカル回転)
	trans.localAngle = MODEL_LOCAL_ROT;

	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

#pragma endregion


#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 衝突時の押し出しを有効にする
	SetPushFlg(true);

	// 押し出す力の大きさを設定する
	SetPushWeight(COLLISION_PUSH_WEIGHT);

#pragma endregion


#pragma region 当たり判定情報設定

	// 当たり判定を生成する（XZ平面上円コライダー）
	ColliderCreate(new XZCircleCollider(COLLIDER_TAG::TOMATO_BOSS_DISTANCE, TO_PLAYER_DISTANCE));

	// 当たり判定を生成する（カプセルコライダー）
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::BOSS,
			CAPSULE_COLLIDER_START_POS_X, CAPSULE_COLLIDER_END_POS_X,
			CAPSULE_COLLIDER_RADIUS,
			CAPSULE_COLLIDER_ENOUGH_DISTANCE
		)
	);
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::BOSS,
			CAPSULE_COLLIDER_START_POS_XZ, CAPSULE_COLLIDER_END_POS_XZ,
			CAPSULE_COLLIDER_RADIUS,
			CAPSULE_COLLIDER_ENOUGH_DISTANCE
		)
	);
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::BOSS,
			CAPSULE_COLLIDER_START_POS_Z, CAPSULE_COLLIDER_END_POS_Z,
			CAPSULE_COLLIDER_RADIUS,
			CAPSULE_COLLIDER_ENOUGH_DISTANCE
		)
	);
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::BOSS,
			CAPSULE_COLLIDER_START_POS_ZX, CAPSULE_COLLIDER_END_POS_ZX,
			CAPSULE_COLLIDER_RADIUS,
			CAPSULE_COLLIDER_ENOUGH_DISTANCE
		)
	);

#pragma endregion


#pragma region プレイヤーが抱える下位クラスを生成する

	subObjArray.push_back(new TomatoStampCollOperator(500.0f, isGround, playerPos, characterStats, *collParam));
	subObjArray.push_back(new TomatoTackleCollOperator(TO_PLAYER_DISTANCE, characterStats, *collParam));
	subObjArray.push_back(new TomatoHeadbuttCollOperator(TO_PLAYER_DISTANCE, characterStats, *collParam));

	// まとめて読み込み処理
	for (ActorBase*& c : subObjArray) { c->Load(); }
#pragma endregion


#pragma region 状態設定

	AddState(
		static_cast<int>(STATE::IDLE),
		new TomatoBossIdleState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::IDLE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::IDLE); },
			// 自分の座標、プレイヤーの座標の読み取り
			trans.pos, *playerPos.at(targetNum),
			// クールタイム
			[&]() { return coolTime; },
			// 頭突きへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::HEADBUTT); },
			// 移動への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::MOVE); },
			// スタンプへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::STAMP); },
			// 突進への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::TACKLE); },
			// 岩に当たっているか
			[&]() { return rockHit; },
			// 岩の当たり判定戻し
			[&]() { rockHit = false; }
			)
	);
	AddState(
		static_cast<int>(STATE::HEADBUTT),
		new TomatoBossHeadbuttState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::HEADBUTT); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::HEADBUTT); },
			// 移動量と攻撃時間
			MOVE_SPEED, 20.0f,
			// 自分の座標と角度、プレイヤーの座標の読み取り
			trans.pos, trans.angle, *playerPos.at(targetNum),
			// コリジョンオペレーターの参照私
			SubObjSerch<TomatoHeadbuttCollOperator>(),
			// XZコライダを消す
			[&]() { for (auto& coll : ColliderSerch(COLLIDER_TAG::BOSS)) { if (coll->GetShape() == SHAPE::XZ_CIRCLE) { coll->SetJudgeFlg(false); } } },
			// XZコライダを戻す
			[&]() { SetJudge(true); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = 120; }
		)
	);
	AddState(
		static_cast<int>(STATE::MOVE),
		new TomatoBossMoveState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::MOVE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::MOVE); },
			// 移動量と回転量
			MOVE_SPEED, ROTATION_POW,
			// 自分の座標と角度、プレイヤーの座標の読み取り
			trans.pos, trans.angle, *playerPos.at(targetNum),
			// 角度を戻す
			[&]() { trans.angle.x = 0; },
			// 移動後攻撃に状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::HEADBUTT); }
			)
	);
	AddState(
		static_cast<int>(STATE::TACKLE),
		new TomatoBossTackleState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::TACKLE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::TACKLE); },
			// 移動量と回転量
			MOVE_SPEED * 5.0f, Deg2Rad(0.3f),
			// 自分の座標と角度、プレイヤーの座標の読み取り
			trans.pos, trans.angle, *playerPos.at(targetNum),
			// コリジョンオペレーターの参照私
			SubObjSerch<TomatoTackleCollOperator>(),
			// 角度を戻す
			[&]() { trans.angle.x = 0; },
			// XZコライダを消す
			[&]() { for (auto& coll : ColliderSerch(COLLIDER_TAG::BOSS)) { if (coll->GetShape() == SHAPE::XZ_CIRCLE) { coll->SetJudgeFlg(false); } } },
			// XZコライダを戻す
			[&]() { SetJudge(true); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = 240; }
			)
	);
	AddState(
		static_cast<int>(STATE::STAMP),
		new TomatoStampState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::STAMP); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::STAMP); },
			// コリジョンオペレーターの参照私
			SubObjSerch<TomatoStampCollOperator>(),
			// 自分の座標の読み取り
			trans.pos, isGround,
			// 最与ダメプレイヤーをターゲットにする
			[&]() { return targetNum; },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// 攻撃時に当たり判定を消すように
			[&]() { SetJudge(false); },
			// 落下中は当たり判定を再生させる
			[&]() { SetJudge(true); },
			// クールタイムの設定
			[&]() { coolTime = 180; }
		)
	);
	
#pragma endregion


#pragma region UI生成
	// HPバー生成
	ui_ArrayIns.emplace_back(
		new CharacterHpUI(
			characterStats.hp,
			characterStats.hpMax.Value(),
			HP_FRAME_IMAGE_NAME,
			HP_IMAGE_NAME,
			HP_LOST_IMAGE_NAME,
			HP_IMAGE_SIZE,
			HP_GAUGE_OFFSET,
			HP_UI_POS,
			FILE_PATH_TYPE::BOSS_HP,
			"TomatoBoss"
		)
	);

	ui_ArrayIns.emplace_back(new HitUI());
#pragma endregion
	ChangeState((int)STATE::IDLE);

}

void TomatoBoss::CharacterInit(void)
{
	// 位置を初期位置にする
	trans.pos = INIT_POS;

	for (ActorBase*& c : subObjArray) { c->Init(); }
}

void TomatoBoss::CharacterUpdate(void)
{
	for (ActorBase*& c : subObjArray) { c->Update(); }

	// ボスのHPがゼロになったとき
	if (characterStats.hp <= 0) {
		// 初めて入ったら画面揺れ
		if (gameOverCnt == 0) {
			GameScene::Shake(ShakeKinds::DIAG, ShakeSize::BIG, GAMECLEAR_CHANGE_TIME);
		}
		gameOverCnt++;
		trans.scale += Vector3(0.75f) * (float)gameOverCnt++;
		ChangeState((int)STATE::IDLE);

		if (gameOverCnt > GAMECLEAR_CHANGE_TIME) {

			gameOverCnt = 0;
			isDeath = true;
		}
	}
}

void TomatoBoss::ReceptionUpdate(void)
{
	while (MsgDataBossTrans* dataPtr = Net::GetIns().GetMsgData<MsgDataBossTrans>(operatorSenderId)) {

		// 座標/角度を同期
		trans.pos = dataPtr->pos;
		trans.angle = dataPtr->angle;
		delete dataPtr;
	}
	while (MsgDataBossInform* dataPtr = Net::GetIns().GetMsgData<MsgDataBossInform>(operatorSenderId)) {

		ChangeState((int)dataPtr->inform);
		delete dataPtr;
	}
	while (MsgDataBossHit* dataPtr = Net::GetIns().GetMsgData<MsgDataBossHit>(operatorSenderId))
	{
		SubUiSerch<HitUI>()->DamageSetting(dataPtr->damage, dataPtr->clitical);
		characterStats.hp -= dataPtr->damage;

		// 与ダメを足す
		damaged[static_cast<int>(Net::GetIns().GetSenderId())] += dataPtr->damage;

		// 与ダメの最大値を探す
		for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
			if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
			if (mostDamaged < damaged.at(id)) {
				mostDamaged = damaged.at(id);
				targetNum = id;
			}
		}

		if (dataPtr->clitical) {
			GameScene::Shake(ShakeKinds::DIAG, ShakeSize::SMALL, 10);
		}
		GameScene::HitStop(4);
		SetInviCounter(150);

		delete dataPtr;
	}
}

void TomatoBoss::SendUpdate(void)
{
	if (Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataBossTrans(trans.pos, trans.angle));
	}
}

void TomatoBoss::CharacterDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->Draw(); }
}

void TomatoBoss::CharacterAlphaDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->AlphaDraw(); }
}

void TomatoBoss::CharacterUiDraw(void)
{
	if (App::GetIns().IsDrawDebug()) {

		// 1行ずつ描画するためのラムダ式（デバッグ用）
		int yPos = 300; const int FONT_SIZE = 20;
		auto debugDrwStr = [&](std::string str)->void {
			DrawStringToHandle(0, yPos, str.c_str(), 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_20));
			yPos += FONT_SIZE;
			};

		// 加速度をデバッグ表示
		debugDrwStr("ボス～～～～～～～～～");
		debugDrwStr("座標" + std::to_string(trans.pos.x) + ", " + std::to_string(trans.pos.y) + ", " + std::to_string(trans.pos.z));
		debugDrwStr("加速度:" + std::to_string(accelSum.Length()));
		debugDrwStr("地面か:" + std::to_string(isGround));
		debugDrwStr("～～～～～～～(|3[___]");
	}
}


void TomatoBoss::CharacterRelease(void)
{
	for (ActorBase*& c : subObjArray) {
		if (c) {
			c->Release();
			delete c;
			c = nullptr;
		}
	}
	subObjArray.clear();
	collParam->Release();
}


void TomatoBoss::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)
{
	if (!Net::GetIns().IsHost()) return;

	if (ownTag == COLLIDER_TAG::TOMATO_BOSS_DISTANCE) {
		switch (other.GetTag()) {
		case COLLIDER_TAG::PLAYER_ATTACK: {
			bool isClitical = false;
			short damage = CalculateDamage(other.GetSkillStats().Power(&isClitical), characterStats.defensePower.Value());

			SubUiSerch<HitUI>()->DamageSetting(damage, isClitical);
			characterStats.hp -= damage;

			if (isClitical) {
				GameScene::Shake(ShakeKinds::DIAG, ShakeSize::SMALL, 10);
			}
			GameScene::HitStop(4);
			SetInviCounter(150);
			
			Net::GetIns().Send(MsgDataBossHit(damage, isClitical));
			break;
		}
		}
		if (other.GetShape() == ColliderBase::SHAPE::XZ_CIRCLE) {
			if (other.GetTag() == COLLIDER_TAG::STAGE) {

				rockHit = true;
			}
		}
	}
}