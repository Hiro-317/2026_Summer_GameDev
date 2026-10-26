#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class BossAttackRangeIndicator final : public ActorBase
{
public:
    enum class SHAPE { Circle, Rectangle, Semicircle };

    /// <summary>
    /// ボスの攻撃範囲インジケーターを作成します。
    /// </summary>
    /// <param name="shape">形状</param>
    /// <param name="sizeX">円形指定時->半径 / 四角形指定時->攻撃範囲のX方向（幅）</param>
    /// <param name="sizeZ">円形指定時->省略 / 四角形指定時->攻撃範囲のZ方向（奥行き）</param>
    BossAttackRangeIndicator(SHAPE shape, float sizeX, float sizeZ = 0.0f);

    ~BossAttackRangeIndicator() override = default;

    // 読み込み
    void Load(void) override;

    // 描画開始
    void Start(void);

    // 描画座標設定
    void SetPos(const Vector3& pos) { trans.pos = pos; }
    // 描画角度設定
    void SetRot(const Quaternion& rot) { trans.rotation = rot; }
    // 攻撃判定開始までの時間割合を設定
    void SetProgress(float progress);

    // 描画終了
    void End(void);

    // ヨー角をラジアン単位で設定
    void SetYaw(float radians) { yaw = radians; }

private:

    void SubInit(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;
    void DrawLayer(int handle, float progressScale, float yOffset) const;

    // 形状
    SHAPE shape;

    // 円形指定時->半径 / 四角形指定時->攻撃範囲のX方向（幅）
    float sizeX;
    // 円形指定時->省略 / 四角形指定時->攻撃範囲のZ方向（奥行き）
    float sizeZ;
    
    // モデルの大きさ（円形指定時->半径 / 四角形指定時->X方向（幅））
    float MODEL_SIZE_X;
    // モデルの大きさ（円形指定時->省略 / 四角形指定時->攻撃範囲のZ方向（奥行き））
    float MODEL_SIZE_Z;

    // 攻撃判定開始までの時間割合
    float progress;

    float yaw;

    bool active;

    int baseModel;
    int fillModel;
};
