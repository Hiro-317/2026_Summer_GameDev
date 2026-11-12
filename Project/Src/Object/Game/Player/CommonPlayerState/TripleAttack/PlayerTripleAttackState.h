#pragma once

#include "../../../../Common/CharacterBase/CharacterStateBase.h"

#include <array>

#include "../../../../../Manager/Input/InputManager.h"

#include "PlayerTripleAttackStDefine.h"

class PlayerTripleAttackCollOperator;

struct Transform;

class PlayerTripleAttackState : public CharacterStateBase
{
public:

	/// <summary>
	/// プレイヤーの3段攻撃状態を管理するクラス
	/// </summary>
	/// <param name="ATTACK_KEY">攻撃に使用するキー。</param>
	/// <param name="COOL_TIME">攻撃そのもののクールタイム（再使用までの待機時間）。</param>
	/// <param name="ATTACK_NEXT_STAGE_CONTINUE_TIME">攻撃が次の段階に繋がる猶予時間。</param>
	/// <param name="COLL_START_TIME">攻撃判定が発生する開始時間（アニメーションの再生割合）。</param>
	/// <param name="COLL_END_TIME">攻撃判定が終了する時間（アニメーションの再生割合）。</param>
	/// <param name="collOperator">当たり判定のオペレーターへの参照。</param>
	/// <param name="playerTrans">プレイヤーのモデル制御情報への参照。</param>
	/// <param name="moveAccel">移動関数のポインタ。</param>
	/// <param name="playAttackAnimes">攻撃1～3段目のアニメーションを再生する関数のポインタ。</param>
	/// <param name="getAnimePlayRatio">アニメーションの再生割合を取得する関数のポインタ。</param>
	/// <param name="defaultChangeState">攻撃終了後の状態遷移を行う関数のポインタ。</param>
	PlayerTripleAttackState(
		KEY_TYPE ATTACK_KEY,

		int COOL_TIME,

		int ATTACK_NEXT_STAGE_CONTINUE_TIME,

		std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max> COLL_START_TIME,
		std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max> COLL_END_TIME,

		PlayerTripleAttackCollOperator& collOperator,

		const Transform& playerTrans,

		std::function<void(const Vector3&)> moveAccel,

		std::array<std::function<void(void)>, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max> playAttackAnimes,
		std::function<float(void)> getAnimePlayRatio,

		std::function<void(void)> defaultChangeState
	);

	~PlayerTripleAttackState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

	// クールタイムを参照
	const float& GetCoolTimeCounter(void)const { return coolTimeCounter; }

private:

#pragma region 定数

	// 攻撃ステップ
	enum class STEP
	{
		// 前隙
		Startup,

		// 攻撃判定発生中
		Active,

		// 後隙
		Recovery
	};

	// 攻撃に使うキー
	KEY_TYPE ATTACK_KEY;

	// 攻撃そのもののクールタイム
	const int COOL_TIME;

	// 攻撃が次段に繋がる猶予時間
	const int ATTACK_NEXT_STAGE_CONTINUE_TIME;

	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max> COLL_START_TIME;
	// 攻撃の判定を発生させる終了時間（アニメーションの再生割合）
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max> COLL_END_TIME;

#pragma endregion


#pragma region 受け取る参照変数・関数

	// 当たり判定のオペレーターの参照
	PlayerTripleAttackCollOperator& collOperator;

	// モデル制御情報の参照
	const Transform& playerTrans;

	// 移動関数のポインタ
	const std::function<void(const Vector3&)> moveAccel;

	// 攻撃1～3段目アニメーションの再生関数のポインタ
	const std::array<std::function<void(void)>, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max> playAttackAnimes;
	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> getAnimePlayRatio;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> defaultChangeState;

#pragma endregion

	// 攻撃ステップ
	STEP step;

	// クールタイム計測
	float coolTimeCounter;

	// 現状の攻撃の段数
	PLAYER_TRIPLE_ATTACK_STAGE attackStage;

	// 攻撃が次段に繋がるかの猶予時間のカウンター
	float attackNextStageContinueTimeCounter;
};