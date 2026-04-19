#pragma once

#include "../../../CharactorStateBase.h"

#include "../../../../../Common/Vector3.h"

#include "PlayerTripleAttackCollOperator.h"

#include "PlayerTripleAttackStDefine.h"

class PlayerTripleAttackState : public CharactorStateBase
{
public:

	/// <summary>
	/// コンストラクト
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="COOL_TIME">攻撃そのもののクールタイム</param>
	/// <param name="ATTACK_NEXT_STAGE_CONTINUE_TIME">次段に繋がるまでの猶予時間</param>
	/// <param name="COLL_START_TIME_1">攻撃の判定を発生させる開始時間（1段目）（アニメーションの再生割合）</param>
	/// <param name="COLL_END_TIME_1">攻撃の判定を発生させる終了時間（1段目）（アニメーションの再生割合）</param>
	/// <param name="COLL_START_TIME_2">攻撃の判定を発生させる開始時間（2段目）（アニメーションの再生割合）</param>
	/// <param name="COLL_END_TIME_2">攻撃の判定を発生させる終了時間（2段目）（アニメーションの再生割合）</param>
	/// <param name="COLL_START_TIME_3">攻撃の判定を発生させる開始時間（3段目）（アニメーションの再生割合）</param>
	/// <param name="COLL_END_TIME_3">攻撃の判定を発生させる終了時間（3段目）（アニメーションの再生割合）</param>
	/// <param name="FIND_ATTACK_TARGET_MOVE_SPEED">攻撃対象が見つかった場合の移動速度</param>
	/// <param name="collOperator">当たり判定のオペレーターの参照</param>
	/// <param name="pos">座標の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="PlayAttack1Anime">攻撃1段目アニメーションの再生関数</param>
	/// <param name="PlayAttack2Anime">攻撃2段目アニメーションの再生関数</param>
	/// <param name="PlayAttack3Anime">攻撃3段目アニメーションの再生関数</param>
	/// <param name="GetAnimePlayRatio">アニメーションの再生割合を取得する関数</param>
	/// <param name="IsAnimeEnd">攻撃終了後の状態遷移関数</param>
	/// <param name="DefaultChangeState">攻撃終了後の状態遷移関数</param>
	PlayerTripleAttackState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,

		int COOL_TIME,
		int ATTACK_NEXT_STAGE_CONTINUE_TIME,
		float COLL_START_TIME_1, float COLL_END_TIME_1,
		float COLL_START_TIME_2, float COLL_END_TIME_2,
		float COLL_START_TIME_3, float COLL_END_TIME_3,
		float ATTACK_MOVE_SPEED,

		PlayerTripleAttackCollOperator& collOperator,
		Vector3& pos, Vector3& angle,

		const std::function<void(void)>& PlayAttack1Anime,
		const std::function<void(void)>& PlayAttack2Anime,
		const std::function<void(void)>& PlayAttack3Anime,

		const std::function<float(void)>& GetAnimePlayRatio,
		const std::function<bool(void)>& IsAnimeEnd,

		const std::function<void(void)>& DefaultChangeState
	);
	~PlayerTripleAttackState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:

#pragma region 定数

	// 攻撃そのもののクールタイム
	const int COOL_TIME;

	// 攻撃が次段に繋がる猶予時間
	const int ATTACK_NEXT_STAGE_CONTINUE_TIME;

	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	const float COLL_START_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::MAX];
	// 攻撃の判定を発生させる終了時間（アニメーションの再生割合）
	const float COLL_END_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::MAX];

	// 攻撃中の移動速度
	const float ATTACK_MOVE_SPEED;

#pragma endregion


#pragma region 受け取る参照変数・関数

	// 当たり判定のオペレーターの参照
	PlayerTripleAttackCollOperator& collOperator;

	// 座標の参照
	Vector3& pos;
	// 角度の参照
	Vector3& angle;

	// 攻撃1～3段目アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayAttackAnimes[(int)PLAYER_TRIPLE_ATTACK_STAGE::MAX];
	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> GetAnimePlayRatio;
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	const std::function<bool(void)> IsAnimeEnd;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;
#pragma endregion

	// 現状の攻撃の段階
	PLAYER_TRIPLE_ATTACK_STAGE attackStage;

	// 攻撃そのものクールタイムのカウンター
	int coolTimeCounter;

	// 攻撃が次段に繋がるかの猶予時間のカウンター
	int attackNextStageContinueTimeCounter;
};