#pragma once

#include "../../../CharactorStateBase.h"

class PlayerDamageState : public CharactorStateBase
{
public:

	// コンストラクタ
	PlayerDamageState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		int DAMAGE_INVI_START_TIME, int DAMAGE_INVI_END_TIME,
		const std::function<void(void)> PlayDamageAnime,
		const std::function<float(void)> GetAnimePlayRatio,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(unsigned char)> SetInviCounter,
		const std::function<void()> DefaultChangeState
	);

	// デストラクタ
	~PlayerDamageState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;


private:

#pragma region 定数
	// 無敵の開始時間（アニメーションの再生割合）
	const int DAMAGE_INVI_START_TIME;

	// 無敵の終了時間（アニメーションの再生割合）
	const int DAMAGE_INVI_END_TIME;
#pragma endregion

#pragma region 関数ポインタ
	// 攻撃アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayDamageAnime;
	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> GetAnimePlayRatio;
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	const std::function<bool(void)> IsAnimeEnd;

	// ダメージ状態終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

	// 無敵カウンターを設定する関数のポインタ
	const std::function<void(unsigned char)> SetInviCounter;
#pragma endregion

};
