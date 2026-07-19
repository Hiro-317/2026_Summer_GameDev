#pragma once

#include"ColliderBase.h"

class SphereCollider : public ColliderBase
{
public:
	SphereCollider(COLLIDER_TAG type, float radius, Vector3 pos = { 0.0f, 0.0f, 0.0f }) :
		ColliderBase(type, pos),
		radius(radius)
	{
		SetShape(SHAPE::SPHERE);
	}
	~SphereCollider()override {}

	float GetRadius(void)const { return radius; }
	void SetRadius(float radius) { this->radius = radius; }

	AABB GetAABB(void) const override {
		Vector3 p = GetPos();
		Vector3 r = radius;

		return AABB(p - r, p + r);
	}

	void DrawDebug(unsigned int color = 0xffffff)override {
		DrawSphere3D(GetPos().ToVECTOR(), radius, 12, color, color, true);
	}

private:
	float radius;
};