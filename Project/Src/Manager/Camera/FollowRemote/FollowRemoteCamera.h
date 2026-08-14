#pragma once

#include "../CameraBase.h"

class FollowRemoteCamera : public CameraBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="targetPos">追従対象</param>
	/// <param name="cameraOffset">カメラ座標を割り出す追従対象からの相対座標</param>
	/// <param name="lookAtOffset">注視点を割り出す追従対象からの相対座標</param>
	/// <param name="ROT_POWER"></param>
	/// <param name="angle"></param>
	/// <param name="fov"></param>
	FollowRemoteCamera(
		const Vector3* targetPos,
		const Vector3& cameraOffset = Vector3::Zonly(-400),
		const Vector3& lookAtOffset = Vector3(),
		float ROT_POWER = 3.0f * (DX_PI_F / 180.0f),
		const Vector3& angle = Vector3(),
		float fov = 60.0f * (DX_PI_F / 180.0f)
	);

	// デストラクタ
	~FollowRemoteCamera()override = default;

#pragma region 主要関数

	// 更新
	void Update(void);

	// デバッグ用描画
	void DrawDebug(void) const;

#pragma endregion

	// 追従対象を途中で変更する
	void TargetChange(const Vector3* targetPos) {
		if (targetPos == nullptr) { return; }
		this->targetPos = targetPos;
	}

private:

	// 追従対象
	const Vector3* targetPos;

	// カメラ座標を割り出す追従対象からの相対座標
	const Vector3& cameraOffset;

	// 注視点を割り出す追従対象からの相対座標
	const Vector3& lookAtOffset;
	// 注視点
	Vector3 lookAtPos;

	// 回転量
	float ROT_POWER;

	// カメラ情報の適用
	void ApplyCameraInfo(void)const;
};