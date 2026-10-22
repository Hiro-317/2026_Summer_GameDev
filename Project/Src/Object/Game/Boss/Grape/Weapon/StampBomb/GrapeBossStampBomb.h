#pragma once
#include "../GrapeBossWeaponBase.h"

class SphereCollider;

class GrapeBossStampBomb : public GrapeBossWeaponBase
{
public:
	GrapeBossStampBomb(int model, int front, int back);
	~GrapeBossStampBomb() = default;

	// ロード
	virtual void Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats) override;

	// 爆発までのカウント
	const float BOMBER_COUNT = GetParameter("StampBomb", "BomberCnt");
	// 攻撃持続時間
	const float ATTACK_DURATION = GetParameter("StampBomb", "Duration");

protected:

	// 派生先追加更新
	virtual void SubUpdate(void) override;

private:

	// 表示倍率を含めないモデルの半径（この数値は攻撃範囲のモデルも同じとする）
	const float MODEL_RADIUS = GetParameter("StampBomb", "Radius");

	// モデル表示倍率
	const float MODEL_SCALE = GetParameter("StampBomb", "Size");

	// 表示倍率も適用した通常時の半径
	const float RADIUS = MODEL_RADIUS * MODEL_SCALE;

	// 攻撃時(爆発範囲)の半径
	const float ATTACK_RADIUS = GetParameter("StampBomb", "Range");

	STATE state;

	// ステージとの当たり判定を行うためのコライダーの参照
	SphereCollider* objectCollRef;
	// プレイヤーに対する攻撃判定を行うためのコライダーの参照
	SphereCollider* attackCollRef;
};
