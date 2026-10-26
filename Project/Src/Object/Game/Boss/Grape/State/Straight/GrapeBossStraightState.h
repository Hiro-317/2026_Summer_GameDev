#pragma once

#include "../../../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Common/Vector3.h"
struct Transform;

class GrapeBossStraightState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標の参照</param>
	/// <param name="angel">角度の参照</param>
	/// <param name="bombType">攻撃種のクラス取得</param>
	/// <param name="playerPos">プレイヤーの座標取得</param>
	/// <param name="GetTarget">プレイヤーの標的番号</param>
	/// <param name="PlayAttackAnim">攻撃アニメーションの再生</param>
	/// <param name="GetAnimPlayRatio">攻撃アニメーションの再生割合取得</param>
	/// <param name="IsAnimeEnd">アニメーションが終了しているか</param>
	/// <param name="DefaultChangeState">攻撃後遷移ステート</param>
	/// <param name="SetCoolTime">クールタイムの設定</param>
	GrapeBossStraightState(
		Transform& bossTrans,
		BombType* bombType,
		const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> PlayAttackAnim,
		const std::function<float(void)> GetAnimPlayRatio,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(void)> DefaultChangeState,
		const std::function<void(void)> SetCoolTime
	);
	~GrapeBossStraightState()override = default;

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

	// 弾の高さ
	static constexpr float HEIGHT = 150.0f;

	// 弾の発射タイミング
	static constexpr float RATIO = 0.3f;

#pragma endregion

#pragma region 受け取る参照変数・関数

	Transform& bossTrans;

	// ボムの参照
	BombType* bombType;

	// プレイヤーの座標の読み取り
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max];

	// プレイヤーの標的番号
	const std::function<int(void)> GetTarget;

	// アタックアニメーションの再生
	const std::function<void(void)> PlayAttackAnim;

	// アニメーションの再生割合取得
	const std::function<float(void)> GetAnimPlayRatio;
	
	// アニメーションが終わっているかどうか
	const std::function<bool(void)> IsAnimeEnd;
	
	// ステートを抜けたあとのステート
	const std::function<void(void)> DefaultChangeState;
	const std::function<void(void)> SetCoolTime;
#pragma endregion

	// 一度だけ通すように
	bool first;

	// プレイヤーターゲット
	int target;

	// 弾の開始角度三つ分
	Vector3 dir[WeponDuplicateNum[(int)WeaponType::Straight]];
};