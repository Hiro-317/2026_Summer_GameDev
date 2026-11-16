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
	/// <param name="IsVisible">ボスが描画されているかを取得する。</param>
	/// <param name="ToggleVisibility">ボスの描画状態を反転する。</param>
	/// <param name="pos">ボスの座標への参照。</param>
	/// <param name="drills">連続ドリル攻撃の管理Actor。</param>
	/// <param name="defaultPos">攻撃終了時に復帰する座標。</param>
	/// <param name="PlayFly">飛行アニメーションを再生する。</param>
	/// <param name="DefaultChangeState">攻撃終了後の待機ステートへ遷移する。</param>
	/// <param name="SetCoolTime">次の攻撃までの待機時間を設定する。</param>
	BananaBossDrillState(
		Vector3& pos,
		BananaBossDrillManager* drills,
		const Vector3& defaultPos,
		const std::function<void(void)> PlayFly,
		const std::function<bool(void)> IsVisible,
		const std::function<void(void)> ToggleVisibility,
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
	const std::function<bool(void)> IsVisible;
	const std::function<void(void)> ToggleVisibility;

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

	GameScene* gameScene = nullptr;
	BananaBossDrillManager* drills;

	bool shouldRestoreFocus = false;

	int hiddenFrames;
};