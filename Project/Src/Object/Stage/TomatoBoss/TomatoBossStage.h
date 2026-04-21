#pragma once

#include"../../ActorBase.h"

class TomatoBossStage : public ActorBase
{
private:

#pragma region 定数定義

    const Vector3 GROUND_COLLISION_SIZE = Vector3(10077.034f, 50.0f, 9234.112f);       // 地面の当たり判定のサイズ
    const Vector3 MODEL_CENTER_DIFF = Vector3::Yonly(-80.0f);       // モデルの描画位置と当たり判定の中心座標の差分
    const Vector3 WALL_LOCAL_ENDPOS = Vector3(0.0f, 1000.0f, 0.0f);        // モデルの描画位置
    const Vector3 ROCK_LOCAL_ENDPOS = Vector3(0.0f, 200.0f, 0.0f);        // モデルの描画位置

    struct ColliderInfo
    {
        Vector3 pos;    // 座標
        float radius;   // 半径
	};

	// 岩の当たり判定の情報（位置と半径）
    const std::vector<ColliderInfo> ROCK_COLLISION_INFO =
    {
        { GetParameterToVector3("RockCollisionPos1") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius1")},
        { GetParameterToVector3("RockCollisionPos2") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius2")},
        { GetParameterToVector3("RockCollisionPos3") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius3")},
        { GetParameterToVector3("RockCollisionPos4") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius4")},
        { GetParameterToVector3("RockCollisionPos5") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius5")},
        { GetParameterToVector3("RockCollisionPos6") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius6")},
        { GetParameterToVector3("RockCollisionPos7") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius7")},
        { GetParameterToVector3("RockCollisionPos8") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius8")},
        { GetParameterToVector3("RockCollisionPos9") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius9")},
        { GetParameterToVector3("RockCollisionPos10") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius10")},
        { GetParameterToVector3("RockCollisionPos11") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius11")},
        { GetParameterToVector3("RockCollisionPos12") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius12")},
        { GetParameterToVector3("RockCollisionPos13") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius13")},
        { GetParameterToVector3("RockCollisionPos14") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius14")},
        { GetParameterToVector3("RockCollisionPos15") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius15")},
        { GetParameterToVector3("RockCollisionPos16") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius16")},
        { GetParameterToVector3("RockCollisionPos17") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius17")},
        { GetParameterToVector3("RockCollisionPos18") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius18")},
        { GetParameterToVector3("RockCollisionPos19") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius19")},
        { GetParameterToVector3("RockCollisionPos20") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius20")},
        { GetParameterToVector3("RockCollisionPos21") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius21")},
        { GetParameterToVector3("RockCollisionPos22") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius22")},
        { GetParameterToVector3("RockCollisionPos23") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius23")},
        { GetParameterToVector3("RockCollisionPos24") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius24")},
        { GetParameterToVector3("RockCollisionPos25") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius25")},
        { GetParameterToVector3("RockCollisionPos26") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius26")},
        { GetParameterToVector3("RockCollisionPos27") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius27")},
        { GetParameterToVector3("RockCollisionPos28") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius28")},
        { GetParameterToVector3("RockCollisionPos29") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius29")},
        { GetParameterToVector3("RockCollisionPos30") + MODEL_CENTER_DIFF, GetParameter("RockCollisionRadius30")},
    };

    const std::vector<ColliderInfo> WALL_COLLISION_INFO = 
    {
        { GetParameterToVector3("WallCollisionPos1") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius1")},
        { GetParameterToVector3("WallCollisionPos2") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius2")},
        { GetParameterToVector3("WallCollisionPos3") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius3")},
        { GetParameterToVector3("WallCollisionPos4") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius4")},
        { GetParameterToVector3("WallCollisionPos5") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius5")},
        { GetParameterToVector3("WallCollisionPos6") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius6")},
        { GetParameterToVector3("WallCollisionPos7") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius7")},
        { GetParameterToVector3("WallCollisionPos8") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius8")},
        { GetParameterToVector3("WallCollisionPos9") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius9")},
        { GetParameterToVector3("WallCollisionPos10") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius10")},
        { GetParameterToVector3("WallCollisionPos11") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius11")},
        { GetParameterToVector3("WallCollisionPos12") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius12")},
        { GetParameterToVector3("WallCollisionPos13") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius13")},
        { GetParameterToVector3("WallCollisionPos14") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius14")},
        { GetParameterToVector3("WallCollisionPos15") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius15")},
        { GetParameterToVector3("WallCollisionPos16") + MODEL_CENTER_DIFF, GetParameter("WallCollisionRadius16")},
    };

#pragma endregion
            
public:

    TomatoBossStage();              // コンストラクタ
    ~TomatoBossStage() = default;   // デストラクタ

    void Load(void)override;    // 読み込み

private:

    void ColliderLoad();    // 当たり判定の読み込み

};