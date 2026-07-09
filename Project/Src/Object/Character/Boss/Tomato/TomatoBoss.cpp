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
#include "State/Stamp/TomatoBossStampState.h"
#include "State/Stamp/TomatoStampCollOperator.h"
#include "State/Death/TomatoBossDeathState.h"

#include "../../../../Manager/Effect/EffectManager.h"

#include "../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../UI/HitUI/HitUI.h"

TomatoBoss::TomatoBoss(const std::vector<const Vector3*> playerPos) :
	BossBase(
		"HP",
		"AttackPower",
		"DefensePower",
		"MoveSpeed",
		"Data/Parameter/Character/Boss/Tomato/",
		"Tomato/Tomato",

		playerPos)
{
	coolTime = 120;

	rockHit = false;
}

void TomatoBoss::PlayerLoad(void)
{

#pragma region 当たり判定情報設定

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


#pragma region トマトが抱える下位クラスを生成する

	subObjArray.push_back(new TomatoHeadbuttCollOperator(TO_PLAYER_DISTANCE, operatorSenderId, characterStats, *collParam));
	subObjArray.push_back(new TomatoStampCollOperator(500.0f, isGround, operatorSenderId, characterStats, *collParam));
	subObjArray.push_back(new TomatoTackleCollOperator(TO_PLAYER_DISTANCE, operatorSenderId, characterStats, *collParam));

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
			trans.pos, playerPos,
			// クールタイム
			[&]() { return coolTime; },
			[&]() { return targetNum; },
			// 頭突きへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); },
			// 移動への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::MOVE); },
			// スタンプへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_B); },
			// 突進への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_C); },
			// 岩に当たっているか
			[&]() { return rockHit; },
			// 岩の当たり判定戻し
			[&]() { rockHit = false; }
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
			trans.pos, trans.angle, playerPos,
			[&]() { return targetNum; },
			// 角度を戻す
			[&]() { trans.angle.x = 0; },
			// 移動後攻撃に状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_A),
		new TomatoBossHeadbuttState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_A); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_A); },
			// 移動量と攻撃時間
			MOVE_SPEED, 20.0f,
			// 自分の座標と角度、プレイヤーの座標の読み取り
			trans.pos, trans.angle, playerPos,
			// コリジョンオペレーターの参照私
			SubObjSerch<TomatoHeadbuttCollOperator>(),
			[&]() { return targetNum; },
			// XZコライダを消す
			[&]() { for (auto& coll : ColliderSerch(COLLIDER_TAG::BOSS)) { if (coll->GetShape() == SHAPE::XZ_CIRCLE) { coll->SetJudgeFlg(false); } } },
			// XZコライダを戻す
			[&]() { SetJudge(true); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = HEADBUTT_COOLTIME; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_B),
		new TomatoBossStampState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_B); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_B); },
			// コリジョンオペレーターの参照私
			SubObjSerch<TomatoStampCollOperator>(),
			// 自分の座標の読み取り、プレイヤーの座標の読み取り
			trans.pos, isGround, playerPos,
			// 最与ダメプレイヤーをターゲットにする
			[&]() { return targetNum; },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// 攻撃時に当たり判定を消すように
			[&]() { SetJudge(false); },
			// 落下中は当たり判定を再生させる
			[&]() { SetJudge(true); },
			// クールタイムの設定
			[&]() { coolTime = STAMP_COOLTIME; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_C),
		new TomatoBossTackleState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_C); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_C); },
			// 移動量と回転量
			MOVE_SPEED * 5.0f, Deg2Rad(0.3f),
			// 自分の座標と角度、プレイヤーの座標の読み取り
			&trans, playerPos,
			// コリジョンオペレーターの参照私
			SubObjSerch<TomatoTackleCollOperator>(),
			[&]() { return targetNum; },
			// 角度を戻す
			[&]() { trans.angle.x = 0; },
			// XZコライダを消す
			[&]() { for (auto& coll : ColliderSerch(COLLIDER_TAG::BOSS)) { if (coll->GetShape() == SHAPE::XZ_CIRCLE) { coll->SetJudgeFlg(false); } } },
			// XZコライダを戻す
			[&]() { SetJudge(true); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = TACKLE_COOLTIME; }
			)
	);
	AddState(
		static_cast<int>(STATE::DEATH),
		new TomatoBossDeathState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::DEATH); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::DEATH); },
			// ボスのサイズ
			trans.scale, MODEL_SCALE,
			// 死んだフラグを立てる
			[&]() { isDeath = true; }
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

}

void TomatoBoss::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)
{
	BossBase::OnCollision(ownTag, other);
	if (ownTag == COLLIDER_TAG::BOSS) {
		if (other.GetTag() == COLLIDER_TAG::STAGE) {
			if (other.GetShape() == ColliderBase::SHAPE::XZ_CIRCLE) {

				rockHit = true;
			}
		}
	}
}

void TomatoBoss::ReceptionUpdate(void)
{
	BossBase::ReceptionUpdate();

	// エリアの同期
	while (MsgDataBossAttackDraw* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDraw>(operatorSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA:
		{
			SubObjSerch<TomatoHeadbuttCollOperator>()->SetViewPos(dataPtr->pos);
			SubObjSerch<TomatoHeadbuttCollOperator>()->SetAngle(dataPtr->angle);
			SubObjSerch<TomatoHeadbuttCollOperator>()->SetScale(dataPtr->scale);
			break;
		}
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackB:
		{
			SubObjSerch<TomatoStampCollOperator>()->SetPos(dataPtr->pos);
			SubObjSerch<TomatoStampCollOperator>()->SetScale(dataPtr->scale.x);
			break;
		}
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC:
		{
			SubObjSerch<TomatoTackleCollOperator>()->SetViewPos(dataPtr->pos);
			SubObjSerch<TomatoTackleCollOperator>()->SetAngle(dataPtr->angle);
			SubObjSerch<TomatoTackleCollOperator>()->SetScale(dataPtr->scale);
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
	while (MsgDataBossAttackDrawFlg* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDrawFlg>(operatorSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA:
		{
			SubObjSerch<TomatoHeadbuttCollOperator>()->SetDrawArea(dataPtr->flg);
			break;
		}
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB:
		{
			SubObjSerch<TomatoStampCollOperator>()->SetDrawArea(dataPtr->flg);
			break;
		}
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC:
		{
			SubObjSerch<TomatoTackleCollOperator>()->SetDrawArea(dataPtr->flg);
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
}