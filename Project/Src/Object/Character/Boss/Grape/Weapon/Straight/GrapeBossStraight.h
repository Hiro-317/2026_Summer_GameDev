#pragma once
#include "../GrapeBossWeaponBase.h"


class GrapeBossStraight : public GrapeBossWeaponBase
{
public:
	GrapeBossStraight(int model);
	~GrapeBossStraight() = default;

	void SetStartPos(Vector3 pos) { trans.pos = pos; }
	void SetStartDir(Vector3 dir) { moveDir = dir; }

	virtual void Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats) override;

protected:

	// ”h¶æ’Ç‰ÁXV
	virtual void SubUpdate(void) override;

private:
#pragma region ’è”’è‹`

	const float MOVE_SPEED = GetParameter("Straight", "Speed");
	const Vector3 DEFAULT_VIEW_SCALE = GetParameter("Straight", "ViewScale");

#pragma endregion

	Vector3 moveDir;
};
