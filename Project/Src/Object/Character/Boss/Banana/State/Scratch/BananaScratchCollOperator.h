#pragma once

#include "../../../../../ActorBase.h"

class BananaScratchCollOperator : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	BananaScratchCollOperator(
		const float TO_PLAYER_DISTANCE,
		const MSG_SENDER_ID operatorSenderId,
		const CharacterStats& stats,
		const ParameterLoad& collParam,
		const std::function<Vector3(void)> StartPos,
		const std::function<Vector3(void)> EndPos
	);
	~BananaScratchCollOperator()override = default;

	void Load(void)override;

	void CollSet(bool flg) { SetJudge(flg); }

	void SetPos(const Vector3& pos) { trans.pos = pos; }
	void SetViewPos(const Vector3& pos) { collBack.pos = Vector3(pos.x, HEIGHT, pos.z); collFront.pos = Vector3(pos.x, HEIGHT, pos.z); }
	void SetAngle(const Vector3& angle) { collBack.angle = angle; collFront.angle = angle; }
	void SetScale(const float& scale) { collFront.scale = Vector3(scale); }

	// 攻撃範囲の描画設定
	void SetDrawArea(bool flg) { isDrawArea = flg; }

private:

	// XZサークルコライダーの半径
	const float TO_PLAYER_DISTANCE;

	// コライダー描画の高さ
	const float HEIGHT = 37.0f;

	// コライダー描画の拡大率
	const Vector3 SCALE = Vector3(5.0f, 1.0f, 2.0f);

#pragma region 受け取る参照変数

	const MSG_SENDER_ID operatorSenderId;

	const CharacterStats& stats;

	const std::function<Vector3(void)> StartPos;

	const std::function<Vector3(void)> EndPos;

#pragma endregion

	void SubAlphaDraw(void) override;

	Transform collBack;
	Transform collFront;

	// 攻撃範囲描画フラグ
	bool isDrawArea;
};