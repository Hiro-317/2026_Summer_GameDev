#pragma once

#include "../../../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../../../Utility/Utility.h"

#include <array>

#include "../../../../../../Manager/Net/SenderID_Define.h"

struct Transform;
struct Vector3;

class GrapeBossIdleState : public CharacterStateBase
{
public:
	
	/// <summary>
	/// グレープボス待機状態
	/// </summary>
	/// <param name="bossTrans"></param>
	/// <param name="playerPos"></param>
	/// <param name="targetNum"></param>
	/// <param name="coolTime"></param>
	/// <param name="playIdleAnim"></param>
	/// <param name="playWalkAnim"></param>
	/// <param name="moveChangeState"></param>
	/// <param name="kickDownChangeState"></param>
	/// <param name="straightChangeState"></param>
	/// <param name="stampChangeState"></param>
	/// <param name="singleChangeState"></param>
	/// <param name="stalkerChangeState"></param>
	/// <param name="randomChangeState"></param>
	GrapeBossIdleState(
		Transform& bossTrans,

		const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],

		const int& targetNum,

		const float& coolTime,

		const std::function<void(void)>& playIdleAnim,
		const std::function<void(void)>& playWalkAnim,

		const std::function<void(void)>& moveChangeState,
		const std::function<void(void)>& kickDownChangeState,
		const std::function<void(void)>& straightChangeState,
		const std::function<void(void)>& stampChangeState,
		const std::function<void(void)>& singleChangeState,
		const std::function<void(void)>& stalkerChangeState,
		const std::function<void(void)>& randomChangeState
	);
	~GrapeBossIdleState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:

#pragma region 受け取る参照変数・関数

	// モデル制御情報の参照
	Transform& bossTrans;

	// プレイヤーの座標の読み取り
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max];

	// ターゲット番号
	const int& targetNum;

	// クールタイムの参照
	const float& coolTime;

	// アイドルアニメーション
	const std::function<void(void)> playIdleAnim;
	// 回転時アニメーション
	const std::function<void(void)> playWalkAnim;

	// 移動へ
	const std::function<void(void)> moveChangeState;
	// キックへ
	const std::function<void(void)> kickDownChangeState;
	// 投擲へ
	const std::function<void(void)> straightChangeState;
	// スタンプへ
	const std::function<void(void)> stampChangeState;
	// 追従へ
	const std::function<void(void)> singleChangeState;
	// たくさんの追従へ
	const std::function<void(void)> stalkerChangeState;
	// ランダムへ
	const std::function<void(void)> randomChangeState;

#pragma endregion

#pragma region 定数定義

	// 確率の全体
	static constexpr int RANDOM = 12000;

	// それぞれの確率
	static constexpr int KICKDOWN_LUCK = 3000;
	static constexpr int STRAIGHT_LUCK = 4000;
	static constexpr int STAMP_LUCK = 6000;
	static constexpr int SINGLE_LUCK = 9000;
	static constexpr int STALKER_LUCK = 11000;
	static constexpr int RELOOT_LUCK = 4500;

	// 移動に移る距離
	static constexpr float DISTANCE = 400.0f;

	// 回らない範囲
	const float NOT_ROT = Deg2Rad(15.0f);
	// 一度に回る角度
	const float ROT_POW = Deg2Rad(2.0f);

#pragma endregion

	// アングル合わせるフラグ
	bool angleFlg;
};