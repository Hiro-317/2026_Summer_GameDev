#pragma once

#include "ColliderBase.h"

class XZCircleCollider : public ColliderBase
{
public:
    XZCircleCollider(COLLIDER_TAG type, float radius, float yLength, Vector3 pos = { 0.0f, 0.0f, 0.0f }) :
        ColliderBase(type, pos),
        radius(radius),
        yLength(yLength)
    {
        SetShape(SHAPE::XZ_CIRCLE);
    }
	~XZCircleCollider()override = default;

	float GetRadius(void)const { return radius; }

	float GetYLength(void)const { return yLength; }

	void SetRadius(float radius) { this->radius = radius; }

	AABB GetAABB(void) const override {
		Vector3 p = GetPos();

        return AABB(
            Vector3(p.x - radius, p.y - yLength, p.z - radius),
            Vector3(p.x + radius, p.y + yLength, p.z + radius)
        );
	}

	void DrawDebug(unsigned int color = 0xffffff)override {
        constexpr int DIV_NUM = 64;

        Vector3 center = GetPos();

        for (int i = 0; i < DIV_NUM; i++) {
            float angle1 = DX_PI_F * 2.0f * i / DIV_NUM;
            float angle2 = DX_PI_F * 2.0f * (i + 1) / DIV_NUM;

            Vector3 p1 = {
                center.x + cosf(angle1) * radius,
                center.y,
                center.z + sinf(angle1) * radius
            };

            Vector3 p2 = {
                center.x + cosf(angle2) * radius,
                center.y,
                center.z + sinf(angle2) * radius
            };

            DrawLine3D(
                VGet(p1.x, p1.y, p1.z),
                VGet(p2.x, p2.y, p2.z),
                color
            );
        }
    }

private:
	// ”¼Œa
	float radius;

	// Y•ûŒü‚Ì’·‚³
	float yLength;
};