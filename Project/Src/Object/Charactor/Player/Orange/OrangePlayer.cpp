#include "OrangePlayer.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Font/FontManager.h"

#include "../CommonPlayerState/Move/PlayerMoveState.h"
#include "../CommonPlayerState/TripleAttack/PlayerTripleAttackState.h"
#include "../CommonPlayerState/SimpleAttack/PlayerSimpleAttackState.h"
#include "../CommonPlayerState/Dodge/PlayerDodgeState.h"
#include "../CommonPlayerState/Damage/PlayerDamageState.h"

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

	// 三弾攻撃の当たり判定オペレーターを生成する
	subObjArray.emplace_back(
		new PlayerTripleAttackCollOperator(
			SKILL_1_TARGET_SERCH_RANGE,
			SKILL_1_COLL_TAG_TABLE,
			SKILL_1_COLL_SIZE_TABLE,
			SKILL_1_COLL_LOCAL_POS,
			trans.pos, trans.angle
		)
	);

	// キック用の当たり判定オペレーターを生成する
	subObjArray.emplace_back(
		new PlayerSimpleAttackCollOperator(
			SKILL_2_TARGET_SERCH_RANGE,
			SKILL_2_COLL_TAG,
			SKILL_2_COLL_SIZE_TABLE,
			SKILL_2_COLL_LOCAL_POS,
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


	// 三弾攻撃状態用のコンテキスト構造体を定義して設定～～～～～～～～～～～
	PlayerTripleAttackStateContext stateSkill1Context;

	// キー
	stateSkill1Context.ATTACK_KEY = KEY_TYPE::PLAYER_SKILL_1;
	// クールタイム
	stateSkill1Context.COOL_TIME = SKILL_1_COOL_TIME;
	// 攻撃が次段に繋がる猶予時間
	stateSkill1Context.ATTACK_NEXT_STAGE_CONTINUE_TIME = SKILL_1_ATTACK_NEXT_STAGE_CONTINUE_TIME;
	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	stateSkill1Context.COLL_START_TIME = SKILL_1_COLL_START_TIME;
	// 攻撃の判定を発生させる終了時間（アニメーションの再生割合）
	stateSkill1Context.COLL_END_TIME = SKILL_1_COLL_END_TIME;
	// 攻撃中の移動速度
	stateSkill1Context.ATTACK_MOVE_SPEED = SKILL_1_ATTACK_MOVE_SPEED;

	// 当たり判定のオペレーター
	stateSkill1Context.collOperator = SubObjSerch<PlayerTripleAttackCollOperator>();

	// 座標
	stateSkill1Context.pos = &trans.pos;
	// 角度
	stateSkill1Context.angle = &trans.angle;

	// 攻撃1～3段目アニメーションの再生関数のポインタ
	stateSkill1Context.PlayAttackAnimes = {
		[&]() { AnimePlay((int)ANIME_TYPE::PUNCH1,false); },
		[&]() { AnimePlay((int)ANIME_TYPE::PUNCH2,false); },
		[&]() { AnimePlay((int)ANIME_TYPE::PUNCH3,false); }
	};
	// アニメーションの再生割合を取得する関数のポインタ
	stateSkill1Context.GetAnimePlayRatio = std::bind(&OrangePlayer::GetAnimeRatio, this);
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	stateSkill1Context.IsAnimeEnd = std::bind(&OrangePlayer::IsAnimeEnd, this);

	// 攻撃終了後の状態遷移関数のポインタ
	stateSkill1Context.DefaultChangeState = [&]() { state = (int)STATE::MOVE; };

	// ～～～～～～～～～～～三弾攻撃状態用のコンテキスト構造体を定義して設定

	// 三段攻撃状態を追加する
	AddState(
		(int)STATE::SKILL_1,
		new PlayerTripleAttackState(
			// 自分の状態に遷移する関数
			[&]() { state = (int)STATE::SKILL_1; },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_1; },
			// コンテキスト構造体
			stateSkill1Context
		)
	);

	// 攻撃状態を追加する (キック)
	AddState(
		(int)STATE::SKILL_2,
		new PlayerSimpleAttackState(
			// 自分の状態に遷移する関数
			[&]() { state = (int)STATE::SKILL_2; },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_2; },
			// 定数（使用するキー / クールタイム / 攻撃の判定を発生させる 開始 / 終了 時間（アニメーションの再生割合）/ 攻撃中の移動速度）
			KEY_TYPE::PLAYER_SKILL_2, SKILL_2_COOL_TIME, SKILL_2_COLL_START_TIME, SKILL_2_COLL_END_TIME, SKILL_2_ATTACK_MOVE_SPEED,
			// 当たり判定のオペレーター
			*SubObjSerch<PlayerSimpleAttackCollOperator>(),
			// 座標 / 角度
			trans.pos, trans.angle,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::KICK, false); },
			// アニメーションの再生割合を取得する関数のポインタ / アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return GetAnimeRatio(); }, [&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { state = (int)STATE::MOVE; }
		)
	);

	AddState(
		(int)STATE::SKILL_3,
		new PlayerDodgeState(
			// 自分の状態に関する関数
			[&]() { state = (int)STATE::SKILL_3; },
			// 自分の状態かどうかを返す関数
			[&]() {return state == (int)STATE::SKILL_3; },
			// 定数（使用するキー / クールタイム / 回避中の移動速度 / 無敵時間の 開始 / 終了時間（アニメーションの再生割合
			KEY_TYPE::PLAYER_SKILL_3, SKILL_3_COOL_TIME, SKILL_3_MOVE_SPEED,
			SKILL_3_INVI_START_TIME, SKILL_3_INVI_END_TIME,
			// 座標 / 角度
			trans.pos,trans.angle,
			[&]() { AnimePlay((int)ANIME_TYPE::DODGE, false); },
			[&]() { return GetAnimeRatio(); },
			[&]() { return IsAnimeEnd(); },
			[&]() { state = (int)STATE::MOVE; },
			std::bind(&OrangePlayer::SetInviCounter, this, std::placeholders::_1)
		)
	);

	AddState(
		(int)STATE::DAMAGE,
		new PlayerDamageState(
			[&]() { state = (int)STATE::DAMAGE; },
			[&]() { return state == (int)STATE::DAMAGE; },

			[&]() { state = (int)STATE::MOVE; }
		)
	);

	// 遷移条件の登録（before = 遷移元)(after = 遷移後）
	auto AddChangeStateCondition = [&](STATE before, STATE after)->void {
		GetStateIns((int)before).AddOtherStateCondition([this, after](void) { GetStateIns((int)after).OwnStateConditionUpdate(); });
		};

	// 移動状態 -> スキル1 の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_1);
	// 移動状態 -> スキル2 の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_2);
	// 移動中 -> スキル3の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_3);

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
}

void OrangePlayer::CharactorAlphaDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->AlphaDraw(); }
}

void OrangePlayer::UiDraw(void)
{
	if (App::GetIns().IsDrawDebug()) {

		// 1行ずつ描画するためのラムダ式（デバッグ用）
		int yPos = 100; const int FONT_SIZE = 20;
		auto debugDrwStr = [&](std::string str)->void {
			DrawStringToHandle(0, yPos, str.c_str(), 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_20));
			yPos += FONT_SIZE;
			};

		// 加速度をデバッグ表示
		debugDrwStr("プレイヤー～～～～～～");
		debugDrwStr("座標" + std::to_string(trans.pos.x) + ", " + std::to_string(trans.pos.y) + ", " + std::to_string(trans.pos.z));	
		debugDrwStr("加速度:" + std::to_string(accelSum.Length()));
		debugDrwStr("スタミナ:" + std::to_string(dynamic_cast<PlayerMoveState&>(GetStateIns((int)STATE::MOVE)).GetDashStamina()));
		debugDrwStr("息切れ:" + std::string(dynamic_cast<PlayerMoveState&>(GetStateIns((int)STATE::MOVE)).IsTired() ? "true" : "false"));
		debugDrwStr("～～～～～～～～～～～");
	}
}

void OrangePlayer::ToDamageState(const int damage, const Vector3& pos)
{
	state = (int)STATE::DAMAGE;
	
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
