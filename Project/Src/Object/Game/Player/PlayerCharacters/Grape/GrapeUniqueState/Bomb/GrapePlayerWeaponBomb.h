#pragma once

#include "../../../../../../Common/ActorBase/ActorBase.h"

struct CharacterStats;

class SphereCollider;

class GrapePlayerWeaponBomb : public ActorBase
{
public:

	GrapePlayerWeaponBomb(
		short ATTACK_RATE_PERCENT,
		const CharacterStats& playerStats,

		float ATTACK_COUNT_TIME,

		const Transform& playerTrans,

		const MSG_SENDER_ID& controlSenderId
	);
	~GrapePlayerWeaponBomb()override = default;

	// ロード
	void Load(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 弾を発射するときに呼び出す処理
	void Put(const Vector3& pos, bool send = false);

	// 弾が爆散したときに呼び出す処理
	void Blast(void);
	void Blast(const Vector3& pos, bool send = false);

	// 同期処理
	void ReceptionUpdate(void)override;

private:

#pragma region 定数

	enum class STATE {
		None,
		Put,
		Blast,
	};

	// 攻撃倍率
	const short ATTACK_RATE_PERCENT;

	// 爆発するまでのカウント時間
	const float ATTACK_COUNT_TIME;

	// 設置状態の判定の半径
	const float PUT_RADIUS = 100.0f;

	// 爆発時判定の半径
	const float BLAST_RADIUS = 1000.0f;

#pragma endregion

#pragma region 受け取る参照変数

	// プレイヤーのステータス
	const CharacterStats& playerStats;

	const Transform playerTrans;

	const MSG_SENDER_ID& controlSenderId;
#pragma endregion

	// 終了までのカウント用
	float lifeCounter;

	STATE state;

	SphereCollider* collRef;

	// 更新処理
	void SubUpdate(void)override;
};