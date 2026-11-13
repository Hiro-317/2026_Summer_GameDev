#pragma once

#include "EffectBase.h"

class RushAuraShader;

// 位置だけを追従し、進行方向の後方へ赤いオーラを流す描画用アクター。
class RushAuraActor : public EffectBase
{
public:
    RushAuraActor(const Transform& target, float radius = 70.0f,
        float trailLength = 350.0f, unsigned int color = 0xff1808);
    void SetDirection(const Vector3& direction);
    void Draw(void) override;

private:
    void DefaultCreateShader(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;
    const Transform& target;
    Vector3 direction;
    float radius;
    float trailLength;
    unsigned int color;
    // 所有権はActorBaseへ渡し、描画前の設定にだけ使用する。
    RushAuraShader* auraShader;
};