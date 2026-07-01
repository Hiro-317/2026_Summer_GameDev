#pragma once
#include "../GrapeBossWeaponBase.h"


class GrapeBossStraight : public GrapeBossWeaponBase
{
public:
	GrapeBossStraight(int model);
	~GrapeBossStraight();

protected:

	// ”h¶æ’Ç‰ÁXV
	virtual void SubUpdate(void) override;
	// ”h¶æ’Ç‰Á•`‰æ
	virtual void SubDraw(void) override;
};
