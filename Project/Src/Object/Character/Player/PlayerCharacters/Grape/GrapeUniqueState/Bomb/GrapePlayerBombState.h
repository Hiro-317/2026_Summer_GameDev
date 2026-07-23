#pragma once

#include "../../../../../CharacterStateBase.h"

#include "GrapePlayerBombCollOperator.h"

class GrapePlayerBombState : public CharacterStateBase
{
public:

	// コンストラクタ
	GrapePlayerBombState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		GrapePlayerBombCollOperator& collOperator,
		const int COOL_TIME,
		const int ATTACK_COUNT_TIME,
		const int ATTACK_START_TIME,
		Vector3& pos,
		const std::function<void(void)> DefaultChangeState
	);

	// デストラクタ
	~GrapePlayerBombState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:
#pragma region 定数定義

	// クールタイム
	const int COOL_TIME;

	// 爆発するまでのカウント時間
	const float ATTACK_COUNT_TIME;

	// 爆弾待機時間
	const float ATTACK_START_TIME; 
#pragma endregion 

#pragma region 変数定義

	// 座標の参照
	Vector3& pos;

	GrapePlayerBombCollOperator& collOperator;

	// 終了までのカウント用
	short timeCounter;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

#pragma endregion 
	bool isInit;
};