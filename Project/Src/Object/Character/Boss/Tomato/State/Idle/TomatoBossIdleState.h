#pragma once

#include "../../../../CharacterStateBase.h"

#include "../../../../../../Common/Vector3.h"

class TomatoBossIdleState : public CharacterStateBase
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
	TomatoBossIdleState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		Vector3& pos, const std::vector<const Vector3*>playerPos,
		const std::function<int(void)> CoolTime,
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> headbuttChangeState,
		const std::function<void(void)> moveChangeState,
		const std::function<void(void)> stampChangeState,
		const std::function<void(void)> tackleChangeState,
		const std::function<bool(void)> hitRock,
		const std::function<void(void)> rockReset
	);
	~TomatoBossIdleState()override = default;

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

#pragma region 受け取る参照変数・関数

	// 座標の参照
	Vector3& pos;

	// プレイヤーの座標の読み取り
	const std::vector<const Vector3*> playerPos;

	const std::function<int(void)> GetTarget;
	// クールタイムの参照
	const std::function<int(void)> CoolTime;

	// 頭突きへ
	const std::function<void(void)> headbuttChangeState;
	// 移動へ
	const std::function<void(void)> moveChangeState;
	// スタンプへ
	const std::function<void(void)> stampChangeState;
	// 突進へ
	const std::function<void(void)> tackleChangeState;

	// 岩との当たり判定
	const std::function<bool(void)> hitRock;
	// 岩との当たり判定の初期化
	const std::function<void(void)> rockReset;

#pragma endregion

	int cnt;

	// プレイヤーターゲット
	int target;
};