#pragma once

#include "../../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../../Manager/Input/InputManager.h"

struct Transform;
struct Vector3;

class PlayerSimpleAttackCollOperator;

class PlayerSimpleAttackState : public CharacterStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ATTACK_KEY">使用するキー</param>
	/// <param name="COOL_TIME">攻撃そのもののクールタイム</param>
	/// <param name="COLL_START_TIME">>攻撃の判定を発生させる開始時間（アニメーションの再生割合）</param>
	/// <param name="COLL_END_TIME">攻撃の判定を発生させる終了時間（アニメーションの再生割合）</param>
	/// <param name="collOperator">当たり判定のオペレーターの参照</param>
	/// <param name="trans">モデル制御情報の参照</param>
	/// <param name="moveAccel">移動関数</param>
	/// <param name="playAttackAnime">攻撃アニメーションの再生関数</param>
	/// <param name="getAnimePlayRatio">アニメーションの再生割合を取得する関数</param>
	/// <param name="defaultChangeState">攻撃終了後の状態遷移関数</param>
	PlayerSimpleAttackState(
		KEY_TYPE ATTACK_KEY,

		int COOL_TIME,

		float COLL_START_TIME,
		float COLL_END_TIME,

		PlayerSimpleAttackCollOperator& collOperator,

		const Transform& trans,

		std::function<void(const Vector3&)> moveAccel,

		std::function<void(void)> playAttackAnime,
		std::function<float(void)> getAnimePlayRatio,
		std::function<void(void)> defaultChangeState
	);
	~PlayerSimpleAttackState() = default;

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
	const KEY_TYPE ATTACK_KEY;

	// 攻撃そのもののクールタイム
	const int COOL_TIME;

	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	const float COLL_START_TIME;
	// 攻撃の判定を発生させる終了時間（アニメーションの再生割合）
	const float COLL_END_TIME;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 当たり判定のオペレーターの参照
	PlayerSimpleAttackCollOperator& collOperator;

	// モデル制御情報の参照
	const Transform& trans;

	// 移動関数のポインタ
	const std::function<void(const Vector3&)> moveAccel;

	// 攻撃アニメーションの再生関数のポインタ
	const std::function<void(void)> playAttackAnime;
	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> getAnimePlayRatio;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> defaultChangeState;

#pragma endregion

	// 攻撃ステップ
	STEP step;

	// クールタイム計測
	float coolTimeCounter;
};