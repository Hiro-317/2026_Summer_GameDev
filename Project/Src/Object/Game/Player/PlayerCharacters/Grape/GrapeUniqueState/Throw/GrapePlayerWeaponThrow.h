#pragma once

#include "../../../../../../Common/ActorBase/ActorBase.h"

struct CharacterStats;
class ExplosionActor;


class GrapePlayerWeaponThrow : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="COLL_TAG">コライダーに着けるタグ</param>
	/// <param name="SKILL_2_ATTACK_RATE_PERCENT">攻撃倍率</param>
	/// <param name="playerPos">プレイヤー座標</param>
	/// <param name="playerAngle">プレイヤーアングルram>
	/// <param name="controlSenderId"></param>
	/// <param name="playerStats">プレイヤーのステータス情報</param>
	GrapePlayerWeaponThrow(
		short ATTACK_RATE_PERCENT,
		const CharacterStats& playerStats,

		const Transform& playerTrans,

		const MSG_SENDER_ID& controlSenderId
	);
	~GrapePlayerWeaponThrow()override = default;

	// ロード
	void Load(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 地面への接地で跳ね返る。
	void SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	// 弾を発射するときに呼び出す処理
	void Shot(const Vector3& pos, const Vector3& vec, bool send = false);

	// 弾が爆散したときに呼び出す処理
	void Blast(void);
	void Blast(const Vector3& pos, bool send = false);

	// 同期処理
	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;

private:

	// 更新処理
	void SubUpdate(void)override;

#pragma region 定数

	enum class STATE {
		None,
		Move,
	};

	// 攻撃倍率
	const short ATTACK_RATE_PERCENT;

	const short LIFE_TIME = 180;

	// スキル1の4倍の大きさと、ゆったり大きく跳ねる専用の重力。
	static constexpr float MODEL_SCALE = 1.0f;
	static constexpr float COLLIDER_RADIUS = 100.0f;
	static constexpr float BOUNCE_SPEED = 15.0f;
	static constexpr float BOUNCE_GRAVITY = 0.65f;

#pragma endregion

#pragma region 受け取る参照変数

	// プレイヤーのステータス
	const CharacterStats& playerStats;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 送信ID
	const MSG_SENDER_ID& controlSenderId;

#pragma endregion

	// 投げる対象（ターゲット）
	Vector3 moveVec;

	// 弾の生存時間
	float lifeCounter;

	STATE state;

	// ExplosionActorが参照する座標を保持し、次の投擲から切り離す。
	Vector3 explosionPosition;
	ExplosionActor* explosion;
	bool hasExploded;
};