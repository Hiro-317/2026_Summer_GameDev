#pragma once

#include "../../Common/ActorBase/ActorBase.h"

#include "../../Common/Common/CharaTypeDefine.h"

class BossSelectSpotArea : public ActorBase
{
public:

	BossSelectSpotArea(float COLLIDER_RADIUS, BOSS_TYPE bossType, bool& selected, const Transform& trans);
	~BossSelectSpotArea()override = default;

	// 読み込み
	void Load(void)override;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

private:

	// 飛行船が使用するコライダーのタグ
	static constexpr COLLIDER_TAG SKY_SHIP_COLL_TAG = COLLIDER_TAG::PLAYER;

	// 押し出しを行うコライダーの半径に対して、選択判定を行う半径の差分
	static constexpr float COLLIDER_RADIUS_EXTRA = 50.0f;

	// コライダーの半径
	const float COLLIDER_RADIUS;

	// このクラスのボスタイプ
	const BOSS_TYPE bossType;

	bool& selected;
};