#pragma once
#include "../../../../../CharacterStateBase.h"

class TomatoPlayerTackleState : public CharacterStateBase
{
public:

	// コンストラクタ
	TomatoPlayerTackleState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		int COOL_TIME,
		float MOVE_SPEED, float ROTATION_POW,
		Vector3& pos, Vector3& angle,
		const std::function<void(void)> DefaultChangeState
	);

	// デストラクタ
	~TomatoPlayerTackleState()override = default;

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

private:
#pragma region 定数定義

	// 移動量
	const float MOVE_SPEED;

	// 回転量
	const float ROTATION_POW;

	// クールタイム
	const int COOL_TIME;

	// チャージ
	static constexpr int CHARGE_POW = 180;

	// 強制突進タイム
	static constexpr int FORCE_MOVE_TIME = 200;

	static constexpr float TACKLE_START_ANGLE = 20.0f;

#pragma endregion 

#pragma region 変数定義

	// 座標の参照
	Vector3& pos;

	// 角度の参照
	Vector3& angle;

	// 向いている方向
	Vector3 moveDir;

	// 終了までのカウント用
	int timeCounter;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

#pragma endregion 

};