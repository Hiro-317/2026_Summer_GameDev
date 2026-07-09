#pragma once
#include "../GrapeBossWeaponBase.h"


class GrapeBossKickBomb : public GrapeBossWeaponBase
{
public:
	GrapeBossKickBomb(int model);
	~GrapeBossKickBomb() = default;

	// ロード
	virtual void Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats) override;

	// 爆発までのカウント
	const float BOMBER_COUNT = GetParameter("KickBomb", "BomberCnt");
	// 攻撃持続時間
	const float ATTACK_DURATION = GetParameter("KickBomb", "Duration");

protected:

	// 派生先追加更新
	virtual void SubUpdate(void) override;

private:

	// 攻撃範囲の描画
	const float ATTACK_RANGE = GetParameter("KickBomb", "Range");

};
