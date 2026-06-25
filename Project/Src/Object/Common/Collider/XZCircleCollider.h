#pragma once

#include "ColliderBase.h"

class XZCircleCollider : public ColliderBase
{
public:
	XZCircleCollider(COLLIDER_TAG type, float radius, float enoughDistance = -1.0f, Vector3 pos = { 0.0f, 0.0f, 0.0f }) :
		ColliderBase(type, enoughDistance, pos),
		radius(radius)
	{
		SetShape(SHAPE::XZ_CIRCLE);
	}
	~XZCircleCollider()override = default;

	float GetRadius(void)const { return radius; }
	void SetRadius(float radius) { this->radius = radius; }

	AABB GetAABB(void) const override {
		Vector3 p = GetPos();

		return AABB(
			Vector3(p.x - radius, p.y, p.z - radius),
			Vector3(p.x + radius, p.y, p.z + radius)
		);
	}

	void DrawDebug(unsigned int color = 0xffffff)override {}

private:
	// ”¼Œa
	float radius;
};