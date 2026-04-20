#include "OrangePlayer.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Font/FontManager.h"

#include "../CommonPlayerState/Move/PlayerMoveState.h"
#include "../CommonPlayerState/TripleAttack/PlayerTripleAttackState.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"

OrangePlayer::OrangePlayer() :
	CharactorBase("Data/Parameter/Charactor/Player/Orange/OrangePlayerParameter.csv"),
	subObjArray()
{
}

void OrangePlayer::Load(void)
{
#pragma region モデル

	// モデルを読み込む
	trans.Load("Charactor/Orange/OrangePlayer");

	// モデルのスケールを設定
	trans.scale = MODEL_SCALE;

	// モデルの中心点のズレ
	trans.centerDiff = MODEL_CENTER_DIFF;

	// 角度のズレ(ローカル回転)
	trans.localAngle = MODEL_LOCAL_ROT;

	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// 各種アニメーションを登録する
	for (int i = 0; i < (int)ANIME_TYPE::MAX; i++) {
		AddAnimation(i, ANIME_SPEED_TABLE[i], ANIME_PATH_TABLE[i].c_str());
	}

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

	// 当たり判定を生成する（線分コライダー）
	ColliderCreate(new LineCollider(TAG::PLAYER, LINE_COLLIDER_START_POS, LINE_COLLIDER_END_POS));

	// 当たり判定を生成する（カプセルコライダー）
	ColliderCreate(
		new CapsuleCollider(
			TAG::PLAYER,
			CAPSULE_COLLIDER_START_POS, CAPSULE_COLLIDER_END_POS,
			CAPSULE_COLLIDER_RADIUS,
			CAPSULE_COLLIDER_ENOUGH_DISTANCE
		)
	);

#pragma endregion


#pragma region プレイヤーが抱える下位クラスを生成する

	subObjArray.emplace_back(
		new PlayerTripleAttackCollOperator(
			SKILL_1_TARGET_SERCH_RANGE,
			SKILL_1_COLL_TAG_TABLE[(int)PLAYER_TRIPLE_ATTACK_STAGE::FIRST],
			SKILL_1_COLL_TAG_TABLE[(int)PLAYER_TRIPLE_ATTACK_STAGE::SECOND],
			SKILL_1_COLL_TAG_TABLE[(int)PLAYER_TRIPLE_ATTACK_STAGE::THIRD],
			SKILL_1_COLL_SIZE_TABLE[(int)PLAYER_TRIPLE_ATTACK_STAGE::FIRST],
			SKILL_1_COLL_SIZE_TABLE[(int)PLAYER_TRIPLE_ATTACK_STAGE::SECOND],
			SKILL_1_COLL_SIZE_TABLE[(int)PLAYER_TRIPLE_ATTACK_STAGE::THIRD],
			SKILL_1_COLL_LOCAL_POS,
			trans.pos, trans.angle
		)
	);

	// まとめて読み込み処理
	for (ActorBase*& c : subObjArray) { c->Load(); }
#pragma endregion



#pragma region 状態設定

	// 移動状態を追加する
    	AddState(
		(int)STATE::MOVE,
		new PlayerMoveState(
			// 自分の状態に遷移する関数
			[&]() { state = (int)STATE::MOVE; },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::MOVE; },
			// 定数（加算移動量 / 移動量の最大値 / ダッシュの移動量倍率 / スタミナ量 / 加速減衰量）
			MOVE_SPEED, MOVE_SPEED_MAX, DASH_SPEED_RATE, DASH_STAMINA_MAX, ATTENUATION,
			// 参照（移動量 / 横軸加速度の最大値 / 角度）
			accelSum, ACCEL_MAX, trans.angle,
			// アニメーションの再生関数のポインタ（待機 / 歩き / 走り）
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE); },
			[&]() { AnimePlay((int)ANIME_TYPE::WALK); },
			[&]() { AnimePlay((int)ANIME_TYPE::RUN); }
		)
	);

	// 三段攻撃状態を追加する
	AddState(
		(int)STATE::SKILL_1,
		new PlayerTripleAttackState(
			// 自分の状態に遷移する関数
			[&]() { state = (int)STATE::SKILL_1; },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_1; },
			// 定数（クールタイム / 次段に繋がるまでの猶予時間）
			SKILL_1_COOL_TIME, SKILL_1_ATTACK_NEXT_STAGE_CONTINUE_TIME,
			// 定数（（1段目）攻撃の判定を発生させる 開始/終了 時間（アニメーションの再生割合））
			SKILL_1_COLL_START_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::FIRST], SKILL_1_COLL_END_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::FIRST],
			// 定数（（2段目）攻撃の判定を発生させる 開始/終了 時間（アニメーションの再生割合））
			SKILL_1_COLL_START_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::SECOND], SKILL_1_COLL_END_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::SECOND],
			// 定数（（3段目）攻撃の判定を発生させる 開始/終了 時間（アニメーションの再生割合））
			SKILL_1_COLL_START_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::THIRD], SKILL_1_COLL_END_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::THIRD],
			// 定数（攻撃対象が見つかった場合の移動速度）
			SKILL_1_ATTACK_MOVE_SPEED,
			// 当たり判定のオペレーター
			*SubObjSerch<PlayerTripleAttackCollOperator>(),
			// 参照（座標 / 角度）
			trans.pos, trans.angle,
			// アニメーションの再生関数のポインタ（1段目 / 2段目 / 3段目）
			[&]() { AnimePlay((int)ANIME_TYPE::PUNCH1, false); },
			[&]() { AnimePlay((int)ANIME_TYPE::PUNCH2, false); },
			[&]() { AnimePlay((int)ANIME_TYPE::PUNCH3, false); },
			// アニメーションの再生割合を取得する関数のポインタ
			[&]() { return GetAnimeRatio(); }, [&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { state = (int)STATE::MOVE; }
		)
	);


	GetStateIns((int)STATE::MOVE).AddOtherStateCondition(
		[this](void) { GetStateIns((int)STATE::SKILL_1).OwnStateConditionUpdate(); }
	);
#pragma endregion
}

void OrangePlayer::CharactorInit(void)
{
	// 位置を初期位置にする
	trans.pos = INIT_POS;

	// 初期状態を移動状態にする
	state = (int)STATE::MOVE;

	for (ActorBase*& c : subObjArray) { c->Init(); }

}

void OrangePlayer::CharactorUpdate(void)
{
	for (ActorBase*& c : subObjArray) { c->Update(); }
}

void OrangePlayer::CharactorDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->Draw(); }

	if (App::GetIns().IsDrawDebug()) {

		// 1行ずつ描画するためのラムダ式（デバッグ用）
		int yPos = 100; const int FONT_SIZE = 20;
		auto debugDrwStr = [&](std::string str)->void {
			DrawStringToHandle(0, yPos, str.c_str(), 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_20));
			yPos += FONT_SIZE;
			};

		// 加速度をデバッグ表示
		debugDrwStr("プレイヤー～～～～～～");
		debugDrwStr("加速度:" + std::to_string(accelSum.Length()));
		debugDrwStr("～～～～～～～～～～～");
	}
}

void OrangePlayer::CharactorAlphaDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->AlphaDraw(); }
}

void OrangePlayer::CharactorRelease(void)
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
