#pragma once

#include "../../../../CharactorStateBase.h"

#include "../../../../../../Common/Vector3.h"

class MoveState : public CharactorStateBase
{
public:
	MoveState(Vector3& accelSum, float ADD_ACCEL);
	~MoveState()override = default;

	void Init(void);
	void Update(void);

private:
	// ˆÚ“®—ÊQÆ
	Vector3& accelSum;

	// ‰ÁZˆÚ“®—Ê
	const float ADD_ACCEL;
};