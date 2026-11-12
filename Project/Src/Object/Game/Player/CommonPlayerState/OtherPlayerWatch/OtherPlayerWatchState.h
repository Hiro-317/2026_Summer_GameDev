#pragma once

#include "../../../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;
struct Transform;

class OtherPlayerWatchState : public CharacterStateBase
{
public:
	OtherPlayerWatchState(const std::vector<const Transform*>& playerTrans, const Vector3*& bossPos);

	~OtherPlayerWatchState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 

private:

	// 座標の参照
	const std::vector<const Transform*>& playerTrans;

	const Vector3*& bossPos;
};

