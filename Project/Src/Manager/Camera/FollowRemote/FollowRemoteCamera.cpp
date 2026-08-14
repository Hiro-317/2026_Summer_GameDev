#include "FollowRemoteCamera.h"

#include "../../../Application/Application.h"

#include "../../Input/KeyManager.h"

FollowRemoteCamera::FollowRemoteCamera(
	const Vector3* targetPos,
	const Vector3& cameraOffset,
	const Vector3& lookAtOffset,
	float ROT_POWER,
	const Vector3& angle,
	float fov
) :
	CameraBase(Vector3(), angle, fov),

	targetPos(targetPos),

	cameraOffset(cameraOffset),

	lookAtOffset(lookAtOffset),
	lookAtPos(),

	ROT_POWER(ROT_POWER)
{
}

void FollowRemoteCamera::Update(void)
{
	// 追従対象が設定されていなかったら処理をしない(安全)
	if (targetPos == nullptr) { return; }

	// 回転処理(コントローラー -> マウス-> ボタン の順に入力を確認していく)

	// コントローラーの右スティックベクトルを代入
	Vector3 rot = Key::GetIns().GetRightStickVec(false).ToVector3YX();

	// コントローラーの右スティックが入力なしならマウスの移動ベクトルを代入
	if (rot == 0.0f) { rot = Key::GetIns().GetMouseMoveSize().ToVector2().ToVector3YX() / MOUSE_SENSI; }

	// マウスが動いてなかったらボタンでの入力を検出してボタンごとに回転方向を 加算/減算 していく
	if (rot == 0.0f) {
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_ROT_RIGHT).now) { rot.y++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_ROT_LEFT).now) { rot.y--; }
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_ROT_FRONT).now) { rot.x--; }
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_ROT_BACK).now) { rot.x++; }
		rot.Normalize();
	}

	// 最終的に入力が1つでもあれば回転させる
	if (rot != 0.0f) {
		angle += rot * ROT_POWER;

		// 回転の数値制御
		if (angle.y <= Deg2Rad(0.0f)) { angle.y += Deg2Rad(360.0f); }
		if (angle.y >= Deg2Rad(360.0f)) { angle.y -= Deg2Rad(360.0f); }
		if (angle.x < Deg2Rad(0.0f)) { angle.x = Deg2Rad(0.0f); }
		if (angle.x > Deg2Rad(60.0f)) { angle.x = Deg2Rad(60.0f); }
	}

	// 現在の追従対象の座標と角度情報から自身(カメラ)の座標を算出する
	pos = *targetPos + lookAtOffset.TransMat(MatrixAllMultXY({ Vector3::XYonly(angle.x,angle.y) }));
	lookAtPos = *targetPos + lookAtOffset.TransMat(MatrixAllMultXY({ Vector3::XYonly(angle.x, angle.y) }));
}

void FollowRemoteCamera::DrawDebug(void) const
{
}

void FollowRemoteCamera::ApplyCameraInfo(void)const
{
	SetCameraPositionAndTarget_UpVecY(pos.ToVECTOR(), lookAtPos.ToVECTOR());
}