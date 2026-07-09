#pragma once
#include "../../../../../CharacterStateBase.h"

#include "../Tackle/TomatoPlayerTackleCollOperator.h"

class TomatoPlayerStampState : public CharacterStateBase
{
public:

	// コンストラクタ
	TomatoPlayerStampState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		//TomatoPlayerTackleCollOperator& collOperator,
		int COOL_TIME,
		float JUMP_POWER,
		Vector3& pos, Vector3& angle,
		const std::function<void(void)> ChangeIsGravity,
		const std::function<bool(void)> IsGround,
		const std::function<void(void)> DefaultChangeState
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

	// チャージ攻撃のUIのためのクールタイム計算関数（チャージ攻撃はここで計算して、UIのチャージ演出をここでしている）
	const int Charge(int& coolTimeCounter, const int COOL_TIME, const int CHARGE_TIME) {
		return  coolTimeCounter += COOL_TIME / CHARGE_TIME;
	}

private:

#pragma region 定数定義

	// ジャンプ力
	const float JUMP_POWER;

	// クールタイム
	const int COOL_TIME;

#pragma endregion 

#pragma region 変数定義

	// 座標の参照
	Vector3& pos;

	// 角度の参照
	Vector3& angle;

	//TomatoPlayerTackleCollOperator& collOperator;

	// 向いている方向
	Vector3 moveDir;

	// タックル終了までのカウント用
	int timeCounter;

	const std::function<void(void)> ChangeIsGravity;
	const std::function<bool(void)> IsGround;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

#pragma endregion 

};