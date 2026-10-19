#pragma once

#include "../../../../Common/CharacterBase/CharacterStateBase.h"

class PlayerDamageState : public CharacterStateBase
{
public:
	
	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="playDamageAnime">アニメーション再生関数</param>
	/// <param name="isAnimeEnd">アニメーションが終了したかどうかを返す関数</param>
	/// <param name="inviStart">無敵判定発生関数</param>
	/// <param name="defaultChangeState">次の状態に遷移する関数</param>
	PlayerDamageState(
		std::function<void(void)> playDamageAnime,
		std::function<bool(void)> isAnimeEnd,
		std::function<void(void)> inviStart,
		std::function<void(void)> defaultChangeState
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

#pragma region 関数ポインタ
	// アニメーションの再生関数のポインタ
	const std::function<void(void)> playDamageAnime;
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	const std::function<bool(void)> isAnimeEnd;

	// 無敵時間のセット関数
	const std::function<void(void)> inviStart;

	// ダメージ状態終了後の状態遷移関数のポインタ
	const std::function<void(void)> defaultChangeState;
#pragma endregion

};
