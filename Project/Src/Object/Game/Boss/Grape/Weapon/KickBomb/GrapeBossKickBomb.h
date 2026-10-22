#pragma once
#include "../GrapeBossWeaponBase.h"

struct CharacterStats;

class GrapeBossKickBomb : public GrapeBossWeaponBase
{
public:
	GrapeBossKickBomb(int model, int front, int back);
	~GrapeBossKickBomb() = default;

	// ロード
	void Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats) override;

	// 爆発までのカウント
	const float BOMBER_COUNT = GetParameter("KickBomb", "BomberCnt");
	// 攻撃持続時間
	const float ATTACK_DURATION = GetParameter("KickBomb", "Duration");

protected:

	// 派生先追加更新
	virtual void SubUpdate(void) override;

private:

	enum class STATE {
		None,

		Fall,

		Ground,

		Blast,
	};

	// 表示倍率を含めないモデルの半径（この数値は攻撃範囲のモデルも同じとする）
	const float MODEL_RADIUS = GetParameter("KickBomb", "Radius");

	// モデル表示倍率
	const float MODEL_SCALE = GetParameter("KickBomb", "Size");

	// 表示倍率も適用した通常時の半径
	const float RADIUS = MODEL_RADIUS * MODEL_SCALE;

	// 攻撃時(爆発範囲)の半径
	const float ATTACK_RADIUS = GetParameter("KickBomb", "Range");

	STATE state;
};
