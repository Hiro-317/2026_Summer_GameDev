#pragma once
#include "../GrapeBossWeaponBase.h"


class GrapeBossStraight : public GrapeBossWeaponBase
{
public:
	GrapeBossStraight(int model, int front, int back);
	~GrapeBossStraight() = default;

	virtual void Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats) override;

	// 接触判定
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 打ち出す
	void Shot(const Vector3& pos, const Vector3& vec);

protected:

	// 派生先追加更新
	virtual void SubUpdate(void) override;

private:
#pragma region 定数定義

	// 球速
	const float MOVE_SPEED = GetParameter("Straight", "Speed");
	// 攻撃サイズ倍率
	const float ATTACK_SIZE = GetParameter("Straight", "Size");

#pragma endregion

	// 移動方向
	Vector3 moveVec;
};
