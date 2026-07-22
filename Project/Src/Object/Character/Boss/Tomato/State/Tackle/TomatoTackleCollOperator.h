#pragma once

#include "../../../../../ActorBase.h"

class TomatoTackleCollOperator : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	TomatoTackleCollOperator(
		const float TO_PLAYER_DISTANCE,
		MSG_SENDER_ID operatorSenderId,
		const CharacterStats& stats,
		const ParameterLoad& collParam
	);
	~TomatoTackleCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

	void CollSet(bool flg) { ColliderSerch(COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(flg); }

	void SetPos(const Vector3& pos) { trans.pos = Vector3(pos.x, HEIGHT, pos.z); }
	void SetViewPos(const Vector3& pos) { collBack.pos = Vector3(pos.x, HEIGHT, pos.z); collFront.pos = Vector3(pos.x, HEIGHT, pos.z); }
	void SetAngle(const float& angle) { collBack.angle.y = collFront.angle.y = angle; }
	void SetScale(const float& scale) { collFront.scale = Vector3(SCALE.x * scale, SCALE.y, SCALE.z); }

	bool GetStageHit(void) { return stageHit; }
	void ResetStageHit(void) { stageHit = false; }

	// 攻撃範囲の描画設定
	void SetDrawArea(bool flg) { isDrawArea = flg; }

private:

	// XZサークルコライダーの半径
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

	bool stageHit;

	int rate;

	Transform collBack;
	Transform collFront;

	// 攻撃範囲描画フラグ
	bool isDrawArea;
};