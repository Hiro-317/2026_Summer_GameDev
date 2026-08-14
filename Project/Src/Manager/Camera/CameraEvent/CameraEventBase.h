#pragma once

class CameraBase;

class CameraEventBase
{
public:

    virtual ~CameraEventBase() = default;

    virtual void Start(CameraBase& camera) {}
    virtual void Update(CameraBase& camera) = 0;
    virtual void End(CameraBase& camera) {}

    virtual bool IsEnd(void) const = 0;
};