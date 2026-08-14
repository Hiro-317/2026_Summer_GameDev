#pragma once

#include"../../Object/Common/Transform.h"

class CameraBase
{
public:

	// カメラのクリップ範囲
	static constexpr float VIEW_NEAR = 10.0f;
	static constexpr float VIEW_FAR = 30000.0f;

	// マウス感度
	static constexpr float MOUSE_SENSI = 35.0f;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="angle">角度</param>
	/// <param name="fov">視野角</param>
	CameraBase(const Vector3& pos = Vector3(), const Vector3& angle = Vector3(), float fov = 60.0f * (DX_PI_F / 180.0f)) : pos(pos), angle(angle), fov(fov) {}
	// デストラクタ
	virtual ~CameraBase() = default;

#pragma region 主要関数

	// 初期化
	virtual void Init(void) {}

	// 更新
	virtual void Update(void) {}

	// 適用
	void Apply(void) {

		// クリップ距離を設定する(ClearDrawScreenでリセットされる)
		SetCameraNearFar(VIEW_NEAR, VIEW_FAR);

		// 視野角を設定する(ClearDrawScreenでリセットされる)
		SetupCamera_Perspective(fov);

		// カメラ情報の適用
		ApplyCameraInfo();
	}

	// デバッグ用描画
	virtual void DrawDebug(void) const {}

	// 終了
	virtual void Release(void) {}

#pragma endregion

#pragma region ゲット関数
	// 座標
	const Vector3& GetPos(void)const { return pos; }
	// 角度
	const Vector3& GetAngle(void)const { return angle; }
	// 視野角
	float GetFov(void)const { return fov; }
#pragma endregion

#pragma region セット関数
	// 座標
	void SetPos(const Vector3& pos) { this->pos = pos; }
	// 角度
	void SetAngle(const Vector3& angle) { this->angle = angle; }
	// 視野角
	void SetFov(float fov) { this->fov = fov; }
#pragma endregion

protected:

	// 座標
	Vector3 pos;

	// 角度
	Vector3 angle;

	// 視野角
	float fov;

	// カメラ情報の適用（派生先で再定義前提）
	virtual void ApplyCameraInfo(void)const { SetCameraPositionAndAngle(pos.ToVECTOR(), angle.x, angle.y, angle.z); }
};