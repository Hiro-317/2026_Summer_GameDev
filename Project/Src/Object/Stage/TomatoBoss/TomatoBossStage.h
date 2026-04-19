#pragma once

#include "../StageBase.h"

class TomatoBossStage : public ActorBase
{
private:
#pragma region 定数定義

    const Vector3 GROUND_COLLISION_SIZE = Vector3(10077.034f, 50.0f, 9234.112f);       // 地面の当たり判定のサイズ
    const Vector3 MODEL_CENTER_DIFF = Vector3::Yonly(-80.0f);       // モデルの描画位置と当たり判定の中心座標の差分

	static constexpr int ROCK_NUM = 12;     // 岩の数

    struct RockColliderInfo
    {
        Vector3 pos; 
        float radius; 
	};

    const RockColliderInfo ROCK_COLLISION_INFO[ROCK_NUM] =
    {
        { Vector3(1241.92f, 100.0f + MODEL_CENTER_DIFF.y,   -341.64f), 150.0f },
        { Vector3(971.69f,  100.0f + MODEL_CENTER_DIFF.y,   22.81f), 100.0f },
        { Vector3(1585.8f,  100.0f + MODEL_CENTER_DIFF.y,   -780.85f), 100.0f },
        { Vector3(503.14f,  100.0f + MODEL_CENTER_DIFF.y,   -1153.06f), 130.0f },
        { Vector3(224.03f,  100.0f + MODEL_CENTER_DIFF.y,   -1193.88f), 50.0f },
        { Vector3(32.91f,   100.0f + MODEL_CENTER_DIFF.y,   -1506.64f), 100.0f },
        { Vector3(-51.34,   100.0f + MODEL_CENTER_DIFF.y,   -1563.09f), 50.0f },
        { Vector3(1038.87f, 100.0f + MODEL_CENTER_DIFF.y,   -1616.43f), 50.0f },
        { Vector3(166.9f,   100.0f + MODEL_CENTER_DIFF.y,   -558.73f), 100.0f },
		{ Vector3(-487.07f,  100.0f + MODEL_CENTER_DIFF.y,   -522.0f), 100.0f },
        { Vector3(-818.21f, 100.0f + MODEL_CENTER_DIFF.y,   -200.16f), 100.0f },
        { Vector3(-882.98f, 100.0f + MODEL_CENTER_DIFF.y,   -298.28f), 100.0f }
    };

#pragma endregion
            
public:

    TomatoBossStage();
    ~TomatoBossStage() = default;

    void Load(void)override;

private:

    // 当たり判定の読み込み
    void ColliderLoad();

};