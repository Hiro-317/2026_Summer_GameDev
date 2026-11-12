#pragma once

#include "../../../../../../Common/ActorBase/ActorBase.h"

struct CharacterStats;


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

	// 派生先追加接地判定（デフォルトはY軸加速度の打ち消し）
	void SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other) {

		// 動的オブジェクトの場合のみ、下方向のY軸加速度のみを打ち消す
		if (GetDynamicFlg()) { velocity.y = (velocity.y < 0.0f) ? -velocity.y : velocity.y; }
	}

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
		Blast,
	};

	// 攻撃倍率
	const short ATTACK_RATE_PERCENT;

	const short LIFE_TIME = 40;

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
};