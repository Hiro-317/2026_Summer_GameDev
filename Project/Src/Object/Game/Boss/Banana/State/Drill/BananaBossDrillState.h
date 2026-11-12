#pragma once

#include "../../../../../Common/CharacterBase/CharacterStateBase.h"
#include "../../../../../../Common/Vector3.h"


class BananaBossDrillManager;
class GameScene;

class BananaBossDrillState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ROTATION_POW">回転量</param>
	BananaBossDrillState(
		Vector3& pos,
		BananaBossDrillManager* drills,
		const Vector3& defaultPos,
		const std::function<void(void)> PlayFly,
		const std::function<bool(void)> Invisible,
		const std::function<void(void)> SwitchInvisible,
		const std::function<void(void)> DefaultChangeState,
		const std::function<void(void)> SetCoolTime
	);
	~BananaBossDrillState()override = default;

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
#pragma region 参照関数

	// 座標の取得
	Vector3& pos;
	// 初期座標の取得
	const Vector3& defaultPos;
	// 死亡判定移行
	const std::function<void(void)> PlayFly;
	// 描画するかフラグ
	const std::function<bool(void)> Invisible;
	const std::function<void(void)> SwitchInvisible;

	const std::function<void(void)> DefaultChangeState;
	const std::function<void(void)> SetCoolTime;
#pragma endregion 参照関数

#pragma region 定数定義

	static constexpr float UP = 75.0f;
	static constexpr float SWITCH_POS = 10000.0f;
	static constexpr float CAMERA_SWITCH_POS = 8000.0f;
	static constexpr int START_CNT = 60;
	static constexpr int END_CNT = START_CNT + 30;

#pragma endregion

	GameScene* scene;
	BananaBossDrillManager* drills;

	bool tmpFlg;

	int cnt;
};