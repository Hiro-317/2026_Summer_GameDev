#pragma once
#include "../GrapeBossWeaponBase.h"


class GrapeBossKickBomb : public GrapeBossWeaponBase
{
public:
	GrapeBossKickBomb(int model);
	~GrapeBossKickBomb() = default;

	void SetStartPos(Vector3 pos) { trans.pos = pos; }

protected:

	// ”h¶æ’Ç‰ÁXV
	virtual void SubUpdate(void) override;

};
