#pragma once

#include "../../../../CharacterStateBase.h"

#include "TomatoTackleCollOperator.h"

#include "../../../../../../Common/Vector3.h"

class TomatoBossTackleState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="ROTATION_POW">回転量</param>
	/// <param name="pos">座標の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	/// <param name="resetAngle">角度を戻す</param>
	/// <param name="DeleteColl">XZのコライダを消す</param>
	/// <param name="ReviveColl">XZのコライダを復活させる</param>
	/// <param name="DefaultChangeState">戻すステート</param>
	TomatoBossTackleState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		float MOVE_SPEED, float ROTATION_POW,
		Vector3& pos, Vector3& angle, const Vector3& playerPos,
		TomatoTackleCollOperator* collOperator,
		const std::function<void(void)> resetAngle,
		const std::function<void(void)> DeleteColl,
		const std::function<void(void)> ReviveColl,
		const std::function<void(void)> DefaultChangeState
	);
	~TomatoBossTackleState()override = default;

	//// 自分の状態に遷移する条件関数
	//void OwnStateConditionUpdate(void);

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

	// 移動量
	const float MOVE_SPEED;

	// 回転量
	const float ROTATION_POW;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 座標の参照
	Vector3& pos;

	// 角度の参照
	Vector3& angle;

	// プレイヤーの座標の読み取り
	const Vector3& playerPos;

	// コリジョンオペレーター
	TomatoTackleCollOperator* collOperator;

	// 角度を戻す
	const std::function<void(void)> resetAngle;

	// XZのコライダを消す
	const std::function<void(void)> DeleteColl;

	// XZのコライダを復活させる
	const std::function<void(void)> ReviveColl;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

#pragma endregion

	Vector3 moveDir;

	float rotPow;

	int time;
};