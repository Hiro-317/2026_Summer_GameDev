#pragma once

#include "../../../../../Common/CharacterBase/CharacterStateBase.h"

#include "GrapeKickDownCollOperator.h"
#include "../../Weapon/GrapeBossWeaponManager.h"

struct Transform;
#include "../../../../../../Common/Vector3.h"

class GrapeBossKickDownState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="LOCAL_ROT">ローカル角度</param>
	/// <param name="bombType">攻撃種のクラス取得</param>
	/// <param name="MODEL_SCALE">モデルの拡大率</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	/// <param name="collOperator">攻撃オペレーターの参照</param>
	/// <param name="GetTarget">ターゲット番号の取得</param>
	/// <param name="PlayAttackAnim">攻撃アニメーションの再生</param>
	/// <param name="GetAnimPlayRatio">攻撃アニメーションの再生割合取得</param>
	/// <param name="IsAnimeEnd">アニメーションが終了しているか</param>
	/// <param name="DefaultChangeState">攻撃後遷移ステート</param>
	/// <param name="SetCoolTime">クールタイムの設定</param>
	GrapeBossKickDownState(
		Transform& bossTrans,
		const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],

		const Vector3 LOCAL_ROT,

		const Vector3& MODEL_SCALE,

		BombType* bombType,
		GrapeKickDownCollOperator* collOperator,

		const int& targetNum,

		const std::function<void(void)>& playAttackAnim,
		const std::function<float(void)>& getAnimPlayRatio,
		const std::function<void(void)>& defaultChangeState,
		const std::function<void(void)>& setCoolTime
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

	// 予測線用のローカル座標
	const Vector3 FOOT_VIEW_POS = Vector3(-23.16f, 0.0f, -85.34f);

	// モデルのローカル回転
	const Vector3 LOCAL_ROT;

	// モデルのスケール
	const Vector3& MODEL_SCALE;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// モデル制御情報の参照
	Transform& bossTrans;

	// プレイヤーの座標の読み取り
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max];

	// ボムの参照
	BombType* bombType;

	// コライダーのオペレーターのポインター
	GrapeKickDownCollOperator* collOperator;

	// ターゲット番号の取得
	const int& targetNum;

	// アタックアニメーションの再生
	const std::function<void(void)> playAttackAnim;

	// アニメーションの再生割合取得
	const std::function<float(void)> getAnimPlayRatio;
	
	// ステートを抜けたあとのステート
	const std::function<void(void)> defaultChangeState;

	// クールタイムセット
	const std::function<void(void)> setCoolTime;

#pragma endregion

	// 一度だけ通すように
	bool first;

	// 攻撃持続のカウント
	int cnt;

	// ターゲット番号の保持
	int target;

	// 攻撃座標
	Vector3 attackPos;
};