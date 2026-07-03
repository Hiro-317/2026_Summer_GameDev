#pragma once
#include "../GrapeBossWeaponBase.h"


class GrapeBossStraight : public GrapeBossWeaponBase
{
public:
	GrapeBossStraight(int model);
	~GrapeBossStraight() = default;

	// 初期設定用関数
	void SetStartDir(Vector3 dir) { moveDir = dir; }

	virtual void Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats) override;

protected:

	// 派生先追加更新
	virtual void SubUpdate(void) override;

private:
#pragma region 定数定義

	const float MOVE_SPEED = GetParameter("Straight", "Speed");
	const Vector3 ATTACK_RANGE = GetParameter("Straight", "Range");

#pragma endregion

	Vector3 moveDir;
};
