#include "TomatoBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Font/FontManager.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"

#include "State/Move/TomatoMoveState.h"
#include "State/Stamp/TomatoStampState.h"
#include "State/Stamp/TomatoStampCollOperator.h"

TomatoBoss::TomatoBoss(const Vector3& playerPos) :
	CharacterBase(1000,500,500,1,"Data/Parameter/Charactor/Boss/Tomato/TomatoBossParameter.csv"),
	subObjArray(),
	playerPos(playerPos)
{
	state = (int)STATE::STAMP;
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

	subObjArray.push_back(new TomatoStampCollOperator(500.0f, 5, isGround, playerPos, characterStats));

	// まとめて読み込み処理
	for (ActorBase*& c : subObjArray) { c->Load(); }
#pragma endregion


#pragma region 状態設定

	AddState(
		static_cast<int>(STATE::MOVE),
		new TomatoMoveState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::MOVE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::MOVE); },
			// 移動量と回転量
			MOVE_SPEED, ROTATION_POW,
			// 自分の座標と角度、プレイヤーの座標の読み取り
			trans.pos, trans.angle, playerPos
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
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { state = (int)STATE::MOVE; },
			// 攻撃時に当たり判定を消すように
			[&]() { SetJudge(false); },
			// 落下中は当たり判定を再生させる
			[&]() { SetJudge(true); },
			// 角度を戻す
			[&]() { trans.angle.x = 0; }

		)
	);

#pragma endregion
}

void TomatoBoss::CharactorInit(void)
{
	// 位置を初期位置にする
	trans.pos = INIT_POS;

	// 初期状態を移動状態にする
	ChangeState(state);

	for (ActorBase*& c : subObjArray) { c->Init(); }

}

void TomatoBoss::CharactorUpdate(void)
{
	for (ActorBase*& c : subObjArray) { c->Update(); }
	if (state == (int)STATE::MOVE) {

		static int i = 0;
		i++;
		if (i > 5) {
			ChangeState((int)STATE::STAMP);
			state = (int)STATE::STAMP;

		}
	}
}

void TomatoBoss::CharactorDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->Draw(); }
}

void TomatoBoss::CharactorAlphaDraw(void)
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


void TomatoBoss::CharactorRelease(void)
{
	for (ActorBase*& c : subObjArray) {
		if (c) {
			c->Release();
			delete c;
			c = nullptr;
		}
	}
	subObjArray.clear();
}
