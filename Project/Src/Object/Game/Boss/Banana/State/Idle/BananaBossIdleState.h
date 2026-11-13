#pragma once

#include "../../../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../../../Common/Vector3.h"
#include "../../../../../../Common/Quaternion.h"

#include "../../../../../../Utility/Utility.h"

#include "../../../../../../Manager/Net/SenderID_Define.h"


class BananaBossIdleState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	/// <param name="CoolTime">クールタイムの取得</param>
	/// <param name="GetTarget">ターゲット番号の取得</param>
	/// <param name="PlayIdleAnim">アイドルアニメーションの再生</param>
	/// <param name="PlayWalkAnim">歩きアニメーションの再生</param>
	/// <param name="MoveChangeState">歩くへのステート</param>
	/// <param name="ScratchChangeState">ひっかきへのステート</param>
	/// <param name="FireChangeState">吐く攻撃へのステート</param>
	/// <param name="BanamerangChangeState">バナメランへのステート</param>
	/// <param name="BanadrillChangeState">バナドリルへのステート</param>
	/// <param name="StalkerChangeState">追従攻撃へのステート</param>
	/// <param name="RandomChangeState">ランダム攻撃へのステート</param>
	BananaBossIdleState(
		Vector3& pos, Quaternion& rotation,
		const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
		const std::function<int(void)> CoolTime,
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> PlayIdleAnim,
		const std::function<void(void)> ScratchChangeState,
		const std::function<void(void)> FireChangeState,
		const std::function<void(void)> BanamerangChangeState,
		const std::function<void(void)> BanadrillChangeState/*,
		const std::function<void(void)> StalkerChangeState,
		const std::function<void(void)> RandomChangeState*/
	);
	~BananaBossIdleState()override = default;

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
	// 角度の参照
	Quaternion& rotation;

	// プレイヤーの座標の読み取り
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max];

	const std::function<int(void)> GetTarget;
	// クールタイムの参照
	const std::function<int(void)> CoolTime;

	// アイドルアニメーション
	const std::function<void(void)> PlayIdleAnim;

	// ひっかきへ
	const std::function<void(void)> ScratchChangeState;
	// 吐くへ
	const std::function<void(void)> FireChangeState;
	// バナメランへ
	const std::function<void(void)> BanamerangChangeState;
	// バナドリルへ
	const std::function<void(void)> BanadrillChangeState;

#pragma endregion

#pragma region 定数定義

	// 確率の全体
	static constexpr int RANDOM = 10000;

	// それぞれの確率
	static constexpr int SCRATCH_LUCK = 3000;
	static constexpr int BREATH_LUCK = 6000;
	static constexpr int BANAMERANG_LUCK = 8500;

	// 移動に移る距離
	static constexpr float DISTANCE = 400.0f;

	// 回らない範囲
	const float NOT_ROT = Deg2Rad(15.0f);
	// 一度に回る角度
	const float ROT_POW = Deg2Rad(2.0f);

#pragma endregion

	// カウント
	int cnt;

	// プレイヤーターゲット
	int target;

	// アングル合わせるフラグ
	bool angleFlg;
};