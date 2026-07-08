#pragma once

#include"ColliderBase.h"

class BoxCollider : public ColliderBase
{
public:
	BoxCollider(COLLIDER_TAG type, Vector3 size, float enoughDistance = -1.0f, Vector3 pos = { 0.0f, 0.0f, 0.0f }) :
		ColliderBase(type, enoughDistance, pos),
		size(size)
	{
		SetShape(SHAPE::BOX);
	}
	~BoxCollider()override {}

	Vector3 GetSize(void)const { return size; }

    AABB GetAABB(void) const override {
        Vector3 p = GetPos();
        Vector3 half = size * 0.5f;

        return AABB(p - half, p + half);
    }

	void DrawDebug(unsigned int color = 0xffffff)override {
        Vector3 center = GetPos();
        Vector3 half = size * 0.5f;

        DrawCube3D((center - half).ToVECTOR(), (center + half).ToVECTOR(), color, color, true);
	}
private:
	Vector3 size;
};