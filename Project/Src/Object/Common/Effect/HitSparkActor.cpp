#include "HitSparkActor.h"

#include "../Shader/HitSparkShader.h"

HitSparkActor::HitSparkActor(float seed) :
    EffectBase(0.42f), seed(seed), critical(false), hitShader(nullptr)
{
    SetDrawType(ACTOR_DRAW_TYPE::Alpha);
}

void HitSparkActor::Emit(const Vector3& point, bool isCritical)
{
    // 一時的な衝突情報への参照を残さず、発生したワールド座標をコピーする。
    SetPos(point);
    critical = isCritical;
    Restart();
}

void HitSparkActor::DefaultCreateShader(void)
{
    hitShader = new HitSparkShader();
    CreateShader(hitShader);
}

void HitSparkActor::Draw(void)
{
    if (!GetIsDraw() || IsFinished() || hitShader == nullptr) { return; }
    hitShader->SetParameters(GetElapsedSeconds(), GetDurationSeconds(), seed, critical);
    ActorBase::Draw();
}

void HitSparkActor::SubDraw(void)
{
    const float radius = critical ? 280.0f : 200.0f;
    const float corners[4][2] = {{-1, 1}, {1, 1}, {-1, -1}, {1, -1}};
    VERTEX3DSHADER vertices[4]{};
    for (int i = 0; i < 4; ++i) {
        vertices[i].pos = trans.pos.ToVECTOR();
        vertices[i].norm = VGet(0.0f, 0.0f, -1.0f);
        vertices[i].dif = GetColorU8(255, 255, 255, 255);
        vertices[i].su = corners[i][0] * radius;
        vertices[i].sv = corners[i][1] * radius;
        vertices[i].u = (corners[i][0] + 1.0f) * 0.5f;
        vertices[i].v = (1.0f - corners[i][1]) * 0.5f;
    }
    const unsigned short indices[6] = {0, 1, 2, 1, 3, 2};
    int previousBlend = 0, previousBlendParameter = 0;
    GetDrawBlendMode(&previousBlend, &previousBlendParameter);
    const int previousCulling = GetUseBackCulling();
    SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 255);
    SetUseBackCulling(DX_CULLING_NONE);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(false);
    DrawPolygonIndexed3DToShader(vertices, 4, indices, 2);
    SetWriteZBuffer3D(true);
    SetUseBackCulling(previousCulling);
    SetDrawBlendMode(previousBlend, previousBlendParameter);
}

void HitSparkActor::SubRelease(void)
{
    hitShader = nullptr;
}