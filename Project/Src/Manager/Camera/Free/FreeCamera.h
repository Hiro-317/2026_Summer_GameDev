#pragma once

#include "../CameraBase.h"

class FreeCamera : public CameraBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="MOVE_POWER">移動速度</param>
	/// <param name="ROT_POWER">回転速度</param>
	/// <param name="pos">初期座標</param>
	/// <param name="angle">初期角度</param>
	/// <param name="fov">視野角</param>
	FreeCamera(float MOVE_POWER, float ROT_POWER, const Vector3& pos = Vector3(), const Vector3& angle = Vector3(), float fov = (60.0f * (DX_PI_F / 180.0f)));

	// デストラクタ
	~FreeCamera()override = default;

private:

	// 移動量
	const float MOVE_POWER;

	// 回転量
	const float ROT_POWER;

	// 更新
	void NormalUpdate(void)override;
};