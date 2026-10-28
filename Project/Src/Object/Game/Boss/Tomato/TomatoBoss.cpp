#include "TomatoBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Sound/SoundManager.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"
#include "../../../Common/Shader/ToonShader.h"

#include "../../../../Scene/Game/GameScene.h"

#include "State/Idle/TomatoBossIdleState.h"
#include "State/Headbutt/TomatoBossHeadbuttState.h"
#include "State/Move/TomatoBossMoveState.h"
#include "State/Tackle/TomatoBossTackleState.h"
#include "State/Stamp/TomatoBossStampState.h"
#include "State/Death/TomatoBossDeathState.h"

#include "../../UI/HpBarUI.h"
#include "../../UI/HitUI.h"

TomatoBoss::TomatoBoss() :
	BossBase(
		"Data/Parameter/Character/Boss/Tomato/",
		"Tomato/Tomato"
	)
{
	coolTime = DEFAULT_COOLTIME;
	rockHit = false;
	Snd::GetIns().AddScene("TomatoBoss");
}

void TomatoBoss::BossLoad(void)
{

#pragma region 当たり判定情報設定

	// 当たり判定を生成する（カプセルコライダー）
	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::Boss,
			CAPSULE_COLLIDER_START_POS_X, CAPSULE_COLLIDER_END_POS_X,
			CAPSULE_COLLIDER_RADIUS
		)
	);
	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::Boss,
			CAPSULE_COLLIDER_START_POS_XZ, CAPSULE_COLLIDER_END_POS_XZ,
			CAPSULE_COLLIDER_RADIUS
		)
	);
	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::Boss,
			CAPSULE_COLLIDER_START_POS_Z, CAPSULE_COLLIDER_END_POS_Z,
			CAPSULE_COLLIDER_RADIUS
		)
	);
	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::Boss,
			CAPSULE_COLLIDER_START_POS_ZX, CAPSULE_COLLIDER_END_POS_ZX,
			CAPSULE_COLLIDER_RADIUS
		)
	);

#pragma endregion


#pragma region トマトが抱える下位クラスを生成する

	headbuttRef = new TomatoHeadbuttCollOperator(GetParameter("Collider", "ToPlayerDistanceRadius"), controlSenderId, characterStats, *collParam);
	AddChildActor(headbuttRef);
	stampRef = new TomatoStampCollOperator(500.0f, isGround, controlSenderId, characterStats, *collParam);
	AddChildActor(stampRef);
	tackleRef = new TomatoTackleCollOperator(GetParameter("Collider", "ToPlayerDistanceRadius"), controlSenderId, characterStats, *collParam);
	AddChildActor(tackleRef);

#pragma endregion


#pragma region 状態設定

	AddState(
		STATE::IDLE,
		new TomatoBossIdleState(
			// 自分の座標、プレイヤーの座標の読み取り
			trans.pos, playerPos,
			// クールタイム
			[&]() { return coolTime; },
			// 現在のターゲット番号
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
		STATE::MOVE,
		new TomatoBossMoveState(
			// 移動量と回転量
			MOVE_SPEED, ROTATION_POW,
			// 自分の座標と角度、プレイヤーの座標の読み取り
			trans, playerPos,
			// 現在のターゲット番号
			[&]() { return targetNum; },
			// 角度を戻す
			[&]() { trans.rotation = Quaternion(); },
			// 移動後攻撃に状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); }
		)
	);
	AddState(
		STATE::ATTACK_A,
		new TomatoBossHeadbuttState(
			// 移動量と攻撃時間
			MOVE_SPEED, 20.0f,
			// 自分の座標と角度、プレイヤーの座標の読み取り
			trans.pos, trans.rotation, playerPos,
			// コリジョンオペレーターの参照渡し
			headbuttRef,
			// 現在のターゲット番号
			[&]() { return targetNum; },
			// XZコライダを消す
			[&]() { for (auto& coll : ColliderSerch(GetColliders(), COLLIDER_TAG::Boss)) { if (coll->GetShape() == COLLIDER_SHAPE::XzCircle) { coll->SetJudgeFlg(false); } } },
			// XZコライダを戻す
			[&]() { SetJudgeFlg(true); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = HEADBUTT_COOLTIME; }
		)
	);
	AddState(
		STATE::ATTACK_B,
		new TomatoBossStampState(
			// コリジョンオペレーターの参照私
			stampRef,
			// 自分の座標の読み取り、プレイヤーの座標の読み取り
			trans.pos, trans.rotation, isGround, playerPos,
			// 現在のターゲット番号
			[&]() { return targetNum; },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// 攻撃時に当たり判定を消すように
			[&]() { SetJudgeFlg(false); },
			// 落下中は当たり判定を再生させる
			[&]() { SetJudgeFlg(true); },
			// クールタイムの設定
			[&]() { coolTime = STAMP_COOLTIME; }
		)
	);
	AddState(
		STATE::ATTACK_C,
		new TomatoBossTackleState(
			// 移動量と回転量
			MOVE_SPEED * 5.0f, Deg2Rad(0.3f),
			// 自分の座標と角度、情報の読み取り,プレイヤーの座標の読み取り
			trans, playerPos,
			// コリジョンオペレーターの参照渡し
			tackleRef,
			// 現在のターゲット番号
			[&]() { return targetNum; },
			// 角度を戻す
			[&]() { trans.rotation = Quaternion::FromRotationY(0.0f); },
			// XZコライダを消す
			[&]() { for (auto& coll : ColliderSerch(GetColliders(), COLLIDER_TAG::Boss)) { if (coll->GetShape() == COLLIDER_SHAPE::XzCircle) { coll->SetJudgeFlg(false); } } },
			// XZコライダを戻す
			[&]() { SetJudgeFlg(true); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = TACKLE_COOLTIME; }
			)
	);
	AddState(
		STATE::DEATH,
		new TomatoBossDeathState(
			// ボスのサイズと角度
			trans.scale, trans.rotation,
			// 死んだフラグを立てる
			[&]() {	isDeath = true; }
			)
	);

#pragma endregion


#pragma region UI生成
	// HPバー生成
	AddChildActor(
		new HpBarUI(
			"Data/Image/Game/UI/Boss/HP/" + HP_FRAME_IMAGE_NAME,
			"Data/Image/Game/UI/Boss/HP/" + HP_IMAGE_NAME,
			"Data/Image/Game/UI/Boss/HP/" + HP_LOST_IMAGE_NAME,

			Vector2I(667, 50),
			Vector2I(629, 50),

			HP_UI_POS,

			characterStats
		)
	);

	AddChildActor(new HitUI());
#pragma endregion

}

void TomatoBoss::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	BossBase::OnCollision(ownTag, other, result);

	// ボスとステージの岩と当たったか
	if (ownTag == COLLIDER_TAG::Boss) {
		if (other.GetTag() == COLLIDER_TAG::Stage) {
			if (other.GetShape() == COLLIDER_SHAPE::XzCircle) {

				rockHit = true;
			}
		}
	}
}

void TomatoBoss::ReceptionUpdate(void)
{
	BossBase::ReceptionUpdate();

	// エリアの同期
	while (MsgDataBossAttackDraw* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDraw>(controlSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA:
		{
			headbuttRef->SetViewPos(dataPtr->pos);
			headbuttRef->SetAngle(dataPtr->angle);
			headbuttRef->SetScale(dataPtr->scale);
			break;
		}
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackB:
		{
			stampRef->SetPos(dataPtr->pos);
			stampRef->SetScale(dataPtr->scale);
			break;
		}
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC:
		{
			tackleRef->SetViewPos(dataPtr->pos);
			tackleRef->SetAngle(dataPtr->angle);
			tackleRef->SetScale(dataPtr->scale);
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
	while (MsgDataBossAttackDrawFlg* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDrawFlg>(controlSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA:
		{
			headbuttRef->SetDrawArea(dataPtr->flg);
			break;
		}
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB:
		{
			if (dataPtr->flg) {
				stampRef->DrawAreaOn();
			}
			else {
				stampRef->Off();
			}
			break;
		}
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC:
		{
			tackleRef->SetDrawArea(dataPtr->flg);
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
}