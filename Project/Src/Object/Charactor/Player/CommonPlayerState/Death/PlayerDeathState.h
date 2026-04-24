#pragma once

#include "../../../CharactorStateBase.h"

class PlayerDeathState : public CharactorStateBase
{
public:
	PlayerDeathState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,

		const std::function<void(void)> PlayDeathAnime,
		const std::function<void(void)> DefaultChangeState
	);
	~PlayerDeathState()override = default;


	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

	const std::function<void(void)> PlayDeathAnime;
	const std::function<void(void)> DefaultChangeState;

};

