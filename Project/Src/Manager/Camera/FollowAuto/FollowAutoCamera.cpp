#include "FollowAutoCamera.h"

#include <algorithm>

#include "../../../Application/Application.h"

#include "../../Input/KeyManager.h"

FollowAutoCamera::FollowAutoCamera(
	const Vector3* targetPos,
	const Vector3* focusPos,
	float TARGET_DISTANCE_MIN, float TARGET_DISTANCE_MAX,
	float fov
) :
	CameraBase(Vector3(), Vector3(), fov),

	targetPos(targetPos),
	focusPos(focusPos),

	TARGET_DISTANCE_MIN(TARGET_DISTANCE_MIN),
	TARGET_DISTANCE_MAX(TARGET_DISTANCE_MAX)
{
}

void FollowAutoCamera::Update(void)
{
	// 追従対象が設定されていなかったら処理をしない(安全)
	if (targetPos == nullptr || focusPos == nullptr) { return; }

	// ２点間ベクトル
	Vector3 atToTarget = *targetPos - *focusPos;

	// fovから必要距離を計算（縦fov基準）
	float needDist = std::clamp((atToTarget.Length() * 0.5f) / tanf(fov * 0.5f), TARGET_DISTANCE_MIN, TARGET_DISTANCE_MAX);

	// lookTargetからみてlookAtのそのさらに先にカメラをおきたいのでその方向を取得する
	Vector3 backDir = atToTarget.Normalized();

	// 目標カメラ位置
	Vector3 desiredPos = *targetPos + backDir * needDist;

	// 高さ補正
	desiredPos.y += std::clamp(((*targetPos - *focusPos) * 0.5f).Length(), 250.0f, 400.0f);

	// 補間（ガタつき防止）
	const float smooth = 0.1f;
	pos += (desiredPos - pos) * smooth;
	if (pos.y <= CAMERA_DOWN) { pos.y = CAMERA_DOWN; }

	// 注視点を算出
	lookAtPos = (*targetPos + *focusPos) * 0.5f;
	if (lookAtPos.y <= FOCUS_DOWN) { lookAtPos.y = FOCUS_DOWN; }

	angle = (lookAtPos - pos);
	angle = Vector3::Yonly(atan2f(angle.x, angle.z));
}

void FollowAutoCamera::DrawDebug(void) const
{
}

void FollowAutoCamera::ApplyCameraInfo(void)const
{
	SetCameraPositionAndTarget_UpVecY(pos.ToVECTOR(), lookAtPos.ToVECTOR());
}