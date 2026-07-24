#pragma once
#include "../../../../../../ActorBase.h"
#include "../../../../../../../Manager/Net/NetWorkManager.h"

class GrapePlayerShotCollOperator : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="COLL_TAG">コライダーのタグ</param>
	/// <param name="ATTACK_RATE_PERCENT">ショットのダメージ倍率</param>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="playerAngle">プレイヤーの角度</param>
	/// <param name="operatorSenderId">送信ID</param>
	/// <param name="playerStats">プレイヤーのステータス</param>
	GrapePlayerShotCollOperator(
		COLLIDER_TAG COLL_TAG,
		const short ATTACK_RATE_PERCENT,
		const Vector3& playerPos, const Vector3& playerAngle,
		MSG_SENDER_ID operatorSenderId,
		const CharacterStats& playerStats
	);

	// デストラクタ
	~GrapePlayerShotCollOperator()override = default;

	// ロード
	void Load(void)override;
	// 更新処理
	void Update(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

	// 攻撃の判定を発生させる
	void CollOn(void) {
		SetJudge(true);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(true, MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerHeadButt));
		}
	}
	// 攻撃の判定を消す
	void CollOff(void) {
		SetJudge(false);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(false, MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerHeadButt));
		}
	}

	// 攻撃のヒット管理のフラグをリセットする
	void ResetIsHit(void) { isHit = false; }

	// 攻撃ヒット管理フラグを取得する
	const bool GetIsHit(void) { return isHit; }

	// 投げる対象のベクトルを取得
	void SetTargetVec(const Vector3& vec) {
		targetVec = vec;
	}
	
	// 初期化処理
	void SetInit(void) {
		ResetIsHit();
		trans.pos = playerPos;
		SetIsDraw(true);
		lifeCounter = LIFE_TIME;
	}
private:

#pragma region 定数

	// 攻撃倍率
	const short ATTACK_RATE_PERCENT;

	// 攻撃の段階ごとのタグ
	COLLIDER_TAG COLL_TAG;

	const short LIFE_TIME = 60;

#pragma endregion

#pragma region 受け取る参照変数

	// プレイヤーの座標
	const Vector3& playerPos;
	// プレイヤーの向き
	const Vector3& playerAngle;

	// 送信ID
	const MSG_SENDER_ID operatorSenderId;

	// プレイヤーのステータス
	const CharacterStats& playerStats;
#pragma endregion

	// 攻撃のヒット管理のフラグ
	bool isHit;

	// 投げる対象（ターゲット）
	Vector3 targetVec;

	// 弾の生存時間
	short lifeCounter;
};