#pragma once

#include "../../../../CharacterStateBase.h"

#include "GrapeKickDownCollOperator.h"
#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Common/Vector3.h"

class GrapeBossKickDownState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="bombType">攻撃種のクラス取得</param>
	/// <param name="model">モデルの取得</param>
	/// <param name="PlayAttackAnim">攻撃アニメーションの再生</param>
	/// <param name="GetAnimPlayRatio">攻撃アニメーションの再生割合取得</param>
	/// <param name="IsAnimeEnd">アニメーションが終了しているか</param>
	/// <param name="DefaultChangeState">攻撃後遷移ステート</param>
	GrapeBossKickDownState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		Vector3& pos, Vector3& angle, const Vector3 LOCAL_ROT,
		BombType* bombType,
		const Vector3& MODEL_SCALE,
		GrapeKickDownCollOperator* collOperator,
		const std::function<void(void)> PlayAttackAnim,
		const std::function<float(void)> GetAnimPlayRatio,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(void)> DefaultChangeState
	);
	~GrapeBossKickDownState()override = default;

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

	// 爆弾の発生高さ
	static constexpr float HEIGHT = 1000.0f;

	// 爆弾のランダム発生距離
	static constexpr int RADIUS = 300;

	// 攻撃の発生持続時間
	static constexpr int ATTACK_DURATION = 5;

	// 攻撃発生させるフレーム
	const char* ATTACK_FRAME = "FEET.R_end";

	// 予測線用のローカル座標
	const Vector3 FOOT_VIEW_POS = Vector3(-23.16f, 0.0f, -85.34f);

	// モデルのローカル回転
	const Vector3 LOCAL_ROT;

	// モデルのスケール
	const Vector3& MODEL_SCALE;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 座標の参照
	Vector3& pos;

	// 角度の参照
	Vector3& angle;
	
	// ボムの参照
	BombType* bombType;

	// コライダーのオペレーターのポインター
	GrapeKickDownCollOperator* collOperator;

	// アタックアニメーションの再生
	const std::function<void(void)> PlayAttackAnim;

	// アニメーションの再生割合取得
	const std::function<float(void)> GetAnimPlayRatio;
	
	// アニメーションが終わっているかどうか
	const std::function<bool(void)> IsAnimeEnd;
	
	// ステートを抜けたあとのステート
	const std::function<void(void)> DefaultChangeState;

#pragma endregion

	// 一度だけ通すように
	bool first;

	// 攻撃持続のカウント
	int cnt;

	// 予測線描画座標
	Vector3 viewPos;
};