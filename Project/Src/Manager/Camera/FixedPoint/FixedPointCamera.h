#pragma once

#include "../CameraBase.h"

class FixedPoint : public CameraBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="angle">角度</param>
	/// <param name="fov">視野角</param>
	FixedPoint(const Vector3& pos, const Vector3& angle, float fov = 60.0f * (DX_PI_F / 180.0f)) : CameraBase(pos, angle, fov) {}

	// デストラクタ
	~FixedPoint()override = default;
};