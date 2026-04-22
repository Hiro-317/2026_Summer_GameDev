#pragma once

#include <functional>

#include "../../../../../Common/Vector3.h"

#include "../../../../../Manager/Input/KeyManager.h"

#include "../../../../Common/Collider/ColliderBase.h"

class PlayerTripleAttackCollOperator;

// 攻撃の段階
enum class	PLAYER_TRIPLE_ATTACK_STAGE
{
	NON = -1,

	FIRST,
	SECOND,
	THIRD,

	MAX
};

// 三弾攻撃の当たり判定オペレーター用のコンテキスト構造体
struct PlayerTripleAttackCollOperatorContext
{
	// 攻撃対象の探索範囲
	float FIND_ATTACK_TARGET_RANGE;

	// 当たり判定のタグテーブル
	TAG COLL_TAG[(int)PLAYER_TRIPLE_ATTACK_STAGE::MAX];
	// 当たり判定のサイズテーブル（半径）
	float COLL_SIZE[(int)PLAYER_TRIPLE_ATTACK_STAGE::MAX];
	// 当たり判定のオフセット
	Vector3 COLL_LOCAL_POS;
	// プレイヤーの座標の参照
	const Vector3& PLAYER_POS;
	// プレイヤーの角度の参照
	const Vector3& PLAYER_ANGLE;
};

// 三弾攻撃のステート用のコンテキスト構造体
struct PlayerTripleAttackStateContext
{
	// 攻撃に使うキー
	KEY_TYPE ATTACK_KEY;

	// 攻撃そのもののクールタイム
	int COOL_TIME;

	// 攻撃が次段に繋がる猶予時間
	int ATTACK_NEXT_STAGE_CONTINUE_TIME;

	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	float COLL_START_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::MAX];
	// 攻撃の判定を発生させる終了時間（アニメーションの再生割合）
	float COLL_END_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::MAX];

	// 攻撃中の移動速度
	float ATTACK_MOVE_SPEED;

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
};