#pragma once

#include "../CameraEventBase.h"

#include "../../../../Object/Common/Transform.h"

class MoveCameraEvent : public CameraEventBase
{
public:

    /// <summary>
    /// 現在のカメラ情報（位置、角度、視野角）から、指定したカメラ情報（位置、角度、視野角）まで指定されたフレーム数で移動する
    /// </summary>
    /// <param name="endPos">終了座標</param>
    /// <param name="endAngle">終了角度</param>
    /// <param name="endFov">終了視野角</param>
    /// <param name="frame">移動フレーム数</param>
    MoveCameraEvent(const Vector3& endPos, const Vector3& endAngle, float endFov, int frame);

    ~MoveCameraEvent() override = default;

    void Start(CameraBase& camera) override;

    void Update(CameraBase& camera) override;

    void End(CameraBase& camera) override;

    bool IsEnd(void) const override { return currentFrame >= maxFrame; }

private:

    // 開始時カメラ情報
    Vector3 startPos;
    Vector3 startAngle;
    float startFov;

    // 終了時カメラ情報
    const Vector3 endPos;
    const Vector3 endAngle;
    const float endFov;

    // 時間
    const int maxFrame;
    int currentFrame;

    // 補間
    float EaseInOut(float t);
};