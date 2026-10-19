#pragma once

#include "../../../../../../Common/CharacterBase/CharacterStateBase.h"

class PlayerSimpleAttackCollOperator;

struct Transform;

class TomatoPlayerHeadButtState : public CharacterStateBase
{
public:

	// コンストラクタ
	TomatoPlayerHeadButtState(

		int COOL_TIME,

		float COLL_START_TIME,
		float COLL_END_TIME,

		PlayerSimpleAttackCollOperator& collOperator,

		Transform& trans,

		std::function<void(void)> defaultChangeState
	);

	// デストラクタ
	~TomatoPlayerHeadButtState()override = default;

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

	// チャージ攻撃のUIのためのクールタイム計算関数（チャージ攻撃はここで計算して、UIのチャージ演出をここでしている）
	const int Charge(int& coolTimeCounter, const int COOL_TIME, const int CHARGE_TIME) {
		return  coolTimeCounter += COOL_TIME / CHARGE_TIME;
	}

private:
#pragma region 定数定義

	// クールタイム
	const int COOL_TIME;

	// 攻撃の判定を発生させる開始時間
	const float COLL_START_TIME;
	// 攻撃の判定を発生させる終了時間
	const float COLL_END_TIME;

#pragma endregion 

#pragma region 変数定義

	// 当たり判定管理クラスの参照
	PlayerSimpleAttackCollOperator& collOperator;

	// プレイヤーのモデル制御情報の参照
	Transform& playerTrans;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> defaultChangeState;

#pragma endregion

	// このステートに遷移してからの時間計測
	float time;

	// クールタイム計測
	float coolTimeCounter;
};