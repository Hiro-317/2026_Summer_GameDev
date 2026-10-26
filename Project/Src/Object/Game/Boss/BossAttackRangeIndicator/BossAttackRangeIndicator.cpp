#include "BossAttackRangeIndicator.h"
#include "BossAttackRangeModelCache.h"
#include "DxLib.h"
#include <algorithm>
#include <stdexcept>

namespace {

    // ベースモデルと塗りつぶしモデルの高さをズラす値
    constexpr float kLayerOffset = 0.015f;

    // 形状ごとのベースモデルパス
    const char* BasePath(BossAttackRangeIndicator::SHAPE shape) {
        switch (shape) {
        case BossAttackRangeIndicator::SHAPE::Circle: return "Data/Model/Range/CircleRangeBack.mv1";
        case BossAttackRangeIndicator::SHAPE::Rectangle: return "Data/Model/Range/LineRangeBack.mv1";
        case BossAttackRangeIndicator::SHAPE::Semicircle: return "Data/Model/Range/CircleHalfRangeBack.mv1";
        }
    }

    // 形状ごとの塗りつぶしモデルパス
    const char* FillPath(BossAttackRangeIndicator::SHAPE shape) {
        switch (shape) {
        case BossAttackRangeIndicator::SHAPE::Circle: return "Data/Model/Range/CircleRangeFill.mv1";
        case BossAttackRangeIndicator::SHAPE::Rectangle: return "Data/Model/Range/LineRangeFill.mv1";
        case BossAttackRangeIndicator::SHAPE::Semicircle: return "Data/Model/Range/CircleHalfRangeFill.mv1";
        }
    }

    float ModelSizeX(BossAttackRangeIndicator::SHAPE shape) {
        switch (shape) {
        case BossAttackRangeIndicator::SHAPE::Circle: return 100.0f;
        case BossAttackRangeIndicator::SHAPE::Rectangle: return 200.0f;
        case BossAttackRangeIndicator::SHAPE::Semicircle: return 100.0f;
        }
    }
    float ModelSizeZ(BossAttackRangeIndicator::SHAPE shape) {
        switch (shape) {
        case BossAttackRangeIndicator::SHAPE::Circle: return 0.0f;
        case BossAttackRangeIndicator::SHAPE::Rectangle: return 200.0f;
        case BossAttackRangeIndicator::SHAPE::Semicircle: return 0.0f;
        }
    }
}

BossAttackRangeIndicator::BossAttackRangeIndicator(SHAPE shape, float sizeX, float sizeZ) :
    shape(shape),
    sizeX(sizeX),
    sizeZ(sizeZ),

    MODEL_SIZE_X(ModelSizeX(shape)),
    MODEL_SIZE_Z(ModelSizeZ(shape)),

    progress(0.0f),

    yaw(0.0f),

    active(false),

    baseModel(-1),
    fillModel(-1)
{
    if (sizeX <= 0.0f || (shape == SHAPE::Rectangle && sizeZ <= 0.0f)) {
        throw std::invalid_argument("Attack range dimensions must be positive");
    }

    SetDynamicFlg(false);
    SetGravityFlg(false);

    SetDrawType(ACTOR_DRAW_TYPE::Alpha);

    SetIsDraw(false);
}

void BossAttackRangeIndicator::Load(void)
{
    if (baseModel >= 0 || fillModel >= 0) { return; }

    baseModel = BossAttackRangeModelCache::Acquire(BasePath(shape));
    try {
        fillModel = BossAttackRangeModelCache::Acquire(FillPath(shape));
    }
    catch (...) {
        BossAttackRangeModelCache::Release(BasePath(shape));
        baseModel = -1;
        throw;
    }
}

void BossAttackRangeIndicator::SubInit(void)
{
    End();
}

void BossAttackRangeIndicator::Start(void)
{
    progress = 0.0f;
    active = true;
    SetIsDraw(true);
}

void BossAttackRangeIndicator::SetProgress(float value)
{
    progress = std::clamp(value, 0.0f, 1.0f);
}

void BossAttackRangeIndicator::End(void)
{
    active = false;
    SetIsDraw(false);
}

void BossAttackRangeIndicator::DrawLayer(int handle, float factor, float yOffset) const
{
    if (handle < 0 || factor <= 0.0f) return;

    const float targetX = shape == SHAPE::Rectangle ? sizeX : sizeX * 2.0f;
    const float targetZ = shape == SHAPE::Rectangle ? sizeZ : sizeX * 2.0f;

    MATRIX world = MGetScale(VGet(targetX / MODEL_SIZE_X * factor, 1.0f, targetZ / MODEL_SIZE_Z * factor));

    const MATRIX rotation = MMult(trans.localRotation.ToMatrix(), MMult(MGetRotY(yaw), trans.rotation.ToMatrix()));

    world = MMult(world, rotation);

    const Vector3 center = trans.pos + Vector3(VTransform(trans.centerDiff.ToVECTOR(), rotation));

    world = MMult(world, MGetTranslate(VGet(center.x, center.y + yOffset, center.z)));

    MV1SetMatrix(handle, world);

    MV1DrawModel(handle);
}

void BossAttackRangeIndicator::SubDraw(void)
{
    if (!active) return;

    DrawLayer(baseModel, 1.0f, 0.0f);
    DrawLayer(fillModel, progress, kLayerOffset);
}

void BossAttackRangeIndicator::SubRelease(void)
{
    if (baseModel >= 0) {
        BossAttackRangeModelCache::Release(BasePath(shape));
        baseModel = -1;
    }
    if (fillModel >= 0) {
        BossAttackRangeModelCache::Release(FillPath(shape));
        fillModel = -1;
    }
}
