#pragma once

#include "../../../../../ActorBase.h"

class TomatoHeadbuttCollOperator : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	TomatoHeadbuttCollOperator(
		const float TO_PLAYER_DISTANCE,
		const MSG_SENDER_ID operatorSenderId,
		const CharacterStats& stats,
		const ParameterLoad& collParam
	);
	~TomatoHeadbuttCollOperator()override = default;

	void Load(void)override;

	void CollSet(bool flg) { SetJudge(flg); }

	void SetPos(const Vector3& pos) { trans.pos = pos; }
	void SetViewPos(const Vector3& pos) { collBack.pos = Vector3(pos.x, HEIGHT, pos.z); collFront.pos = Vector3(pos.x, HEIGHT, pos.z); }
	void SetAngle(const Vector3& angle) { collBack.angle = angle; collFront.angle = angle; }
	void SetScale(const Vector3& scale) { collFront.scale = Vector3(SCALE.x * scale.x, SCALE.y, SCALE.z); }

	// 攻撃範囲の描画設定
	void SetDrawArea(bool flg) { isDrawArea = flg; }

private:

	// カプセルコライダーのローカル座標
	const float TO_PLAYER_DISTANCE;

	// コライダー描画の高さ
	const float HEIGHT;

	// コライダー描画の拡大率
	const Vector3 SCALE;

	// コライダー描画の中心
	const Vector3 CENTER;

	// コライダー描画のローカル回転
	const Vector3 ANGLE;

#pragma region 受け取る参照変数

	const MSG_SENDER_ID operatorSenderId;

	const CharacterStats& stats;

#pragma endregion

	void SubAlphaDraw(void) override;

	Transform collBack;
	Transform collFront;

	// 攻撃範囲描画フラグ
	bool isDrawArea;
};