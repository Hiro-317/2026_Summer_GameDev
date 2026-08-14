#include "CameraBase.h"

#include "../../Application/Application.h"

#include "CameraEvent/CameraEventBase.h"

CameraBase::CameraBase(const Vector3& pos, const Vector3& angle, float fov) :
	pos(pos), angle(angle), fov(fov),
	cameraEvent(nullptr)
{
}

void CameraBase::Update(void)
{
	// カメライベントがあればそれを更新する
	if (cameraEvent) {
		cameraEvent->Update(*this);

		if (cameraEvent->IsEnd()) {
			cameraEvent->End(*this);

			//OnCameraEventEnd();
		}

		// カメライベント中は通常の更新はしない
		return;
	}

	// なければ通常の更新
	NormalUpdate();
}

void CameraBase::Apply(void) 
{
	// クリップ距離を設定する(ClearDrawScreenでリセットされる)
	SetCameraNearFar(VIEW_NEAR, VIEW_FAR);

	// 視野角を設定する(ClearDrawScreenでリセットされる)
	SetupCamera_Perspective(fov);

	// カメラ情報の適用
	SetCameraPositionAndAngle(pos.ToVECTOR(), angle.x, angle.y, angle.z);
}

void CameraBase::DrawDebug(void) const
{
	if (App::GetIns().IsDrawDebug()) {
		DrawFormatString(
			0, 70, 0xffffff,
			"カメラ座標　 ：(% .1f, % .1f, % .1f)",
			pos.x, pos.y, pos.z
		);
		DrawFormatString(
			0, 90, 0xffffff,
			"カメラ角度　 ：(% .1f, % .1f, % .1f)",
			Rad2Deg(angle.x),
			Rad2Deg(angle.y),
			Rad2Deg(angle.z)
		);
	}
}


#pragma region イベント関数

void CameraBase::StartEvent(CameraEventBase* event)
{
	if (event == nullptr) { return; }

	cameraEvent = event;
	cameraEvent->Start(*this);
}

#pragma endregion
