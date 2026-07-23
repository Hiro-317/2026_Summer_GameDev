#pragma once

#include"../../ActorBase.h"

class GrapeBossStage : public ActorBase
{
private:

#pragma region 定数定義

    // 地面の当たり判定のサイズ
    const Vector3 GROUND_COLLISION_SIZE = Vector3(10077.034f, 90.0f, 9234.112f);
    // モデルの描画位置と当たり判定の中心座標の差分
    const Vector3 MODEL_CENTER_DIFF = Vector3::Yonly(-110.0f);

    // 壁の高さ
    const float WALL_HEIGHT = 1000.0f;

    struct ColliderInfo
    {
        Vector3 pos;    // 座標
        float side;   // 壁の一辺
        float angle;    //壁の角度
	};

    const std::vector<ColliderInfo> WALL_COLLISION_INFO =
    {
    };

#pragma endregion
            
public:

    GrapeBossStage();              // コンストラクタ
    ~GrapeBossStage() = default;   // デストラクタ

    void Load(void)override;    // 読み込み

protected:

    void SubDraw(void)override;
    void SubRelease(void)override;

private:

    void ColliderLoad();    // 当たり判定の読み込み

    Transform sky;      // スカイドーム用
};