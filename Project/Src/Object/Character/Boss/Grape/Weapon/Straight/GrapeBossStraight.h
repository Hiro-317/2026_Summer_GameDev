#pragma once
#include "../GrapeBossWeaponBase.h"


class GrapeBossStraight : public GrapeBossWeaponBase
{
public:
	GrapeBossStraight(int model);
	~GrapeBossStraight() = default;

	void SetStartPos(Vector3 pos) { trans.pos = pos; }
	void SetStartDir(Vector3 dir) { moveDir = dir; }

protected:

	// ”h¶æ’Ç‰ÁXV
	virtual void SubUpdate(void) override;
	// ”h¶æ’Ç‰Á•`‰æ
	virtual void SubDraw(void) override;

private:
#pragma region ’è”’è‹`

	const float MOVE_SPEED = GetParameter("Straight", "Speed");

#pragma endregion


	Vector3 moveDir;
};
