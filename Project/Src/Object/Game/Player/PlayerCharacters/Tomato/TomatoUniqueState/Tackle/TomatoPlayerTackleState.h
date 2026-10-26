#pragma once

#include "../../../../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../../../../Common/Vector3.h"

class PlayerSimpleAttackCollOperator;

struct Transform;

class TomatoPlayerTackleState : public CharacterStateBase
{
public:

	// コンストラクタ
	TomatoPlayerTackleState(

		PlayerSimpleAttackCollOperator& collOperator,

		Transform& playerTrans,

		const std::function<void(void)> chargeAttackBuff,
		const std::function<void(void)> chargeAttackBuffDelete,
		const std::function<void(void)> defaultChangeState
	);

	// デストラクタ
	~TomatoPlayerTackleState()override = default;

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

	const float& GetCoolTimeCounter(void)const { return coolTimeCounter; }
private:

#pragma region 定数定義

	enum class STEP {
		Charge,
		Tackle
	};

	// 移動量
	const float MOVE_SPEED = 20.0f;

	// 回転量
	const float ROTATION_POW = 0.1f;

	// クールタイム
	const int COOL_TIME = 200;

	// このスタート角度まで回転したら、突進をスタートする
	static constexpr float TACKLE_START_ANGLE = 20.0f;

	// 突進してから終了までのカウントタイム
	static constexpr int COUNT_MAX = 20;

	// チャージ時間
	static constexpr int CHARGE_TIME = 60;

#pragma endregion 

#pragma region 変数定義

	// 座標の参照
	Transform& playerTrans;

	PlayerSimpleAttackCollOperator& collOperator;

	// 向いている方向
	Vector3 moveDir;

	// タックル終了までのカウント用
	int timeCounter;

	// チャージ用カウンタ
	int chargeCounter;

	// チャージした時、ダメージを増やすための関数のポインタ
	const std::function<void(void)> chargeAttackBuff;
	const std::function<void(void)> chargeAttackBuffDelete;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> defaultChangeState;

#pragma endregion 

	STEP step;

	float coolTimeCounter;


	// タックル処理
	void Tackle(void);

	// 移動方向入力取得
	Vector3 InputVec(void)const;
};