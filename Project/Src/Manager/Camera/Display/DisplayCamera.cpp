#include "DisplayCamera.h"

#include "../../../Application/Application.h"

#include "../../Input/KeyManager.h"

DisplayCamera::DisplayCamera(const Vector3& fixedLookAtPos, const Vector3& lookAtDiff, float ROT_POWER, const Vector3& angle, float fov) :
	CameraBase(Vector3(), angle, fov),

	fixedLookAtPos(fixedLookAtPos),
	lookAtDiff(lookAtDiff),

	ROT_POWER(ROT_POWER)
{
}

void DisplayCamera::Update(void)
{
	// 回転処理（設定された値横向きに回し続ける）
	angle += Vector3::Yonly(1.0f).Normalized() * ROT_POWER;

	if (angle.y >= Deg2Rad(360.0f)) { angle.y -= Deg2Rad(360.0f); }
	if (angle.y <= Deg2Rad(0.0f)) { angle.y += Deg2Rad(360.0f); }

	// 現在の追従対象の座標と角度情報から自身(カメラ)の座標を算出する
	pos = fixedLookAtPos + lookAtDiff.TransMat(MatrixAllMultXY({ Vector3::XYonly(angle.x,angle.y) }));
}

void DisplayCamera::DrawDebug(void) const
{

}

void DisplayCamera::ApplyCameraInfo(void)const
{
	SetCameraPositionAndTarget_UpVecY(pos.ToVECTOR(), fixedLookAtPos.ToVECTOR());
}