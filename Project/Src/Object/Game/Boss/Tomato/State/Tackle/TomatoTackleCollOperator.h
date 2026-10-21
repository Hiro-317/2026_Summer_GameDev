#pragma once

#include "../../../../../Common/ActorBase/ActorBase.h"

class XZCircleCollider;

struct CharacterStats;

class TomatoTackleCollOperator : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	TomatoTackleCollOperator(
		const float TO_PLAYER_DISTANCE,

		const MSG_SENDER_ID& controlSenderId,

		const CharacterStats& stats,

		const ParameterLoad& collParam
	);
	~TomatoTackleCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	void CollSet(bool flg) { ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetJudgeFlg(flg); }

	void SetPos(const Vector3& pos) { trans.pos = Vector3(pos.x, HEIGHT, pos.z); }
	void SetViewPos(const Vector3& pos) { collBack.pos = Vector3(pos.x, HEIGHT, pos.z); collFront.pos = Vector3(pos.x, HEIGHT, pos.z); }
	void SetAngle(const float& angle) { collBack.rotation = collFront.rotation = Quaternion::FromRotationY(angle); }
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

	const MSG_SENDER_ID& controlSenderId;

	const CharacterStats& stats;

#pragma endregion

	void SubDraw(void) override;
	void SubRelease(void) override;

	// 攻撃判定用当たり判定管理クラスの参照
	XZCircleCollider* attackCollRef;
	// ステージとの押し出し用当たり判定管理クラスの参照
	XZCircleCollider* stagePushCollRef;

	// 攻撃範囲モデル制御情報
	Transform collBack;
	Transform collFront;

	// ステージとの当たり判定管理フラグ
	bool stageHit;

	// 
	int rate;

	// 攻撃範囲描画フラグ
	bool isDrawArea;
};