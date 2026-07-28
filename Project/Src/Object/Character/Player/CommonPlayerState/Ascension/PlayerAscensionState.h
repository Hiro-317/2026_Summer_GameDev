#pragma once
#include "../../../CharacterStateBase.h"

class PlayerAscensionState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="own
	/// ">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="INVI_TIME">無敵時間</param>
	/// <param name="PlayDamageAnime">アニメーション再生関数</param>
	/// <param name="IsAnimeEnd">アニメーションが終了したかどうかを返す関数</param>
	/// <param name="DefaultChangeState">次の状態に遷移する関数</param>
	PlayerAscensionState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		const std::function<void(void)> DefaultChangeState
	);

	// デストラクタ
	~PlayerAscensionState()override = default;

	void Enter(void)override;
	void Update(void)override;
	void Exit(void)override;


private:

#pragma region 関数ポインタ
	// ダメージ状態終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;
#pragma endregion

};