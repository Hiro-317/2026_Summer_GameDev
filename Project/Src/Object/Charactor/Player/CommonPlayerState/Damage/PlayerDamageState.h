#pragma once

#include "../../../CharactorStateBase.h"

class PlayerDamageState : public CharactorStateBase
{
public:
	PlayerDamageState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		
		const std::function<void()> DefaultChangeState
	);

	~PlayerDamageState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;


private:

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

	int nowDamageCounter;

};
