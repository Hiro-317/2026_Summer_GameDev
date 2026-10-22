#pragma once

#include "../../../../../../Common/CharacterBase/CharacterStateBase.h"

class PlayerSimpleAttackCollOperator;

struct Transform;
struct Vector3;

class TomatoPlayerStampState : public CharacterStateBase
{
public:

	// コンストラクタ
	TomatoPlayerStampState(
		const bool& isGround,

		PlayerSimpleAttackCollOperator& collOperator,

		const std::function<void(const Vector3&)>& moveAccel,
		float& accelMax,

		Vector3& velocity,

		const std::function<void(bool)>& setIsGravity,
		const std::function<void(void)>& defaultChangeState
	);

	// デストラクタ
	~TomatoPlayerStampState()override = default;

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

	// クールタイム取得
	const float& GetCoolTimeCounter(void)const { return coolTimeCounter; }

private:

#pragma region 受け取る定数定義

	// クールタイム
	const int COOL_TIME = 300;

	// ジャンプ力
	const float JUMP_POWER = 50.0f;

	// ジャンプ速度の減衰量（重力）
	const float GRAVITY = 0.6f;

	// ジャンプ中の 加速度の最大値
	const float JUMP_MOVE_SPEED = 4.0f;

	// 加速度の最大値の規定値
	const float DEFAULT_ACCEL_MAX;

#pragma endregion 

#pragma region 受け取る用

	// 接地判定
	const bool& isGround;

	// 当たり判定管理クラス
	PlayerSimpleAttackCollOperator& collOperator;

	// 移動関数
	const std::function<void(const Vector3&)> moveAccel;

	// 加速度の最大値
	float& accelMax;

	// 加速度の参照
	Vector3& velocity;

	// プレイヤーの重力のオンオフ切り替え関数
	const std::function<void(bool)> setIsGravity;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> defaultChangeState;

#pragma endregion 

	// 攻撃終了判定用
	bool isEnd;

	// クールタイム計測
	float coolTimeCounter;

	// ジャンプ中の移動処理
	void JumpMove(void);
};