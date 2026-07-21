#pragma once

#include"../../ActorBase.h"

class GrapeBossStage : public ActorBase
{
private:

#pragma region 定数定義

    // 地面の当たり判定のサイズ
    const Vector3 GROUND_COLLISION_SIZE = Vector3(10077.034f, 50.0f, 9234.112f);
    // モデルの描画位置と当たり判定の中心座標の差分
    const Vector3 MODEL_CENTER_DIFF = Vector3::Yonly(-80.0f);
    // モデルの描画位置
    const Vector3 WALL_LOCAL_ENDPOS = Vector3(0, -100, 0);
    // モデルの描画位置
    const Vector3 ROCK_LOCAL_ENDPOS = Vector3(0, -100, 0);

    struct ColliderInfo
    {
        Vector3 pos;    // 座標
        float radius;   // 半径
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

private:

    void ColliderLoad();    // 当たり判定の読み込み

    Transform sky;      // スカイドーム用
};