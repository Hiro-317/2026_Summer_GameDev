#pragma once

#include"../../ActorBase.h"

class TomatoBossStage : public ActorBase
{
private:

#pragma region 定数定義

    // 地面の当たり判定のサイズ
    const Vector3 GROUND_COLLISION_SIZE = Vector3(10077.034f, 50.0f, 9234.112f);
    // モデルの描画位置と当たり判定の中心座標の差分
    const Vector3 MODEL_CENTER_DIFF = Vector3::Yonly(-80.0f);
    // モデルの描画位置
    const Vector3 WALL_LOCAL_ENDPOS = Vector3(0.0f, 1000.0f, 0.0f);
    // モデルの描画位置
    const Vector3 ROCK_LOCAL_ENDPOS = Vector3(0.0f, -100.0f, 0.0f);

    struct ColliderInfo
    {
        Vector3 pos;    // 座標
        float radius;   // 半径
        float height;
	};

	// 岩の当たり判定の情報（位置と半径）
    const std::vector<ColliderInfo> ROCK_COLLISION_INFO =
    {
        { GetParameterToVector3("RockCollisionPos1") + MODEL_CENTER_DIFF,   GetParameter("RockCollisionRadius1"),   GetParameter("RockCollisionHeight1")    },
        { GetParameterToVector3("RockCollisionPos2") + MODEL_CENTER_DIFF,   GetParameter("RockCollisionRadius2"),   GetParameter("RockCollisionHeight2")    },
        { GetParameterToVector3("RockCollisionPos3") + MODEL_CENTER_DIFF,   GetParameter("RockCollisionRadius3"),   GetParameter("RockCollisionHeight3")    },
        { GetParameterToVector3("RockCollisionPos4") + MODEL_CENTER_DIFF,   GetParameter("RockCollisionRadius4"),   GetParameter("RockCollisionHeight4")    },
        { GetParameterToVector3("RockCollisionPos5") + MODEL_CENTER_DIFF,   GetParameter("RockCollisionRadius5"),   GetParameter("RockCollisionHeight5")    },
        { GetParameterToVector3("RockCollisionPos6") + MODEL_CENTER_DIFF,   GetParameter("RockCollisionRadius6"),   GetParameter("RockCollisionHeight6")    },
        { GetParameterToVector3("RockCollisionPos7") + MODEL_CENTER_DIFF,   GetParameter("RockCollisionRadius7"),   GetParameter("RockCollisionHeight7")    },
        { GetParameterToVector3("RockCollisionPos8") + MODEL_CENTER_DIFF,   GetParameter("RockCollisionRadius8"),   GetParameter("RockCollisionHeight8")    },
        { GetParameterToVector3("RockCollisionPos9") + MODEL_CENTER_DIFF,   GetParameter("RockCollisionRadius9"),   GetParameter("RockCollisionHeight9")    },
        { GetParameterToVector3("RockCollisionPos10") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius10"),  GetParameter("RockCollisionHeight10")   },
        { GetParameterToVector3("RockCollisionPos11") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius11"),  GetParameter("RockCollisionHeight11")   },
        { GetParameterToVector3("RockCollisionPos12") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius12"),  GetParameter("RockCollisionHeight12")   },
        { GetParameterToVector3("RockCollisionPos13") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius13"),  GetParameter("RockCollisionHeight13")   },
        { GetParameterToVector3("RockCollisionPos14") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius14"),  GetParameter("RockCollisionHeight14")   },
        { GetParameterToVector3("RockCollisionPos15") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius15"),  GetParameter("RockCollisionHeight15")   },
        { GetParameterToVector3("RockCollisionPos16") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius16"),  GetParameter("RockCollisionHeight16")   },
        { GetParameterToVector3("RockCollisionPos17") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius17"),  GetParameter("RockCollisionHeight17")   },
        { GetParameterToVector3("RockCollisionPos18") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius18"),  GetParameter("RockCollisionHeight18")   },
        { GetParameterToVector3("RockCollisionPos19") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius19"),  GetParameter("RockCollisionHeight19")   },
        { GetParameterToVector3("RockCollisionPos20") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius20"),  GetParameter("RockCollisionHeight20")   },
        { GetParameterToVector3("RockCollisionPos21") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius21"),  GetParameter("RockCollisionHeight21")   },
        { GetParameterToVector3("RockCollisionPos22") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius22"),  GetParameter("RockCollisionHeight22")   },
        { GetParameterToVector3("RockCollisionPos23") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius23"),  GetParameter("RockCollisionHeight23")   },
        { GetParameterToVector3("RockCollisionPos24") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius24"),  GetParameter("RockCollisionHeight24")   },
        { GetParameterToVector3("RockCollisionPos25") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius25"),  GetParameter("RockCollisionHeight25")   },
        { GetParameterToVector3("RockCollisionPos26") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius26"),  GetParameter("RockCollisionHeight26")   },
        { GetParameterToVector3("RockCollisionPos27") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius27"),  GetParameter("RockCollisionHeight27")   },
        { GetParameterToVector3("RockCollisionPos28") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius28"),  GetParameter("RockCollisionHeight28")   },
        { GetParameterToVector3("RockCollisionPos29") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius29"),  GetParameter("RockCollisionHeight29")   },
        { GetParameterToVector3("RockCollisionPos30") + MODEL_CENTER_DIFF,  GetParameter("RockCollisionRadius30"),  GetParameter("RockCollisionHeight30")   },
    };

    const std::vector<ColliderInfo> WALL_COLLISION_INFO = 
    {
        { GetParameterToVector3("WallCollisionPos1") + MODEL_CENTER_DIFF,   GetParameter("WallCollisionRadius1"),    GetParameter("WallCollisionHeight1")    },
        { GetParameterToVector3("WallCollisionPos2") + MODEL_CENTER_DIFF,   GetParameter("WallCollisionRadius2"),    GetParameter("WallCollisionHeight2")    },
        { GetParameterToVector3("WallCollisionPos3") + MODEL_CENTER_DIFF,   GetParameter("WallCollisionRadius3"),    GetParameter("WallCollisionHeight3")    },
        { GetParameterToVector3("WallCollisionPos4") + MODEL_CENTER_DIFF,   GetParameter("WallCollisionRadius4"),    GetParameter("WallCollisionHeight4")    },
        { GetParameterToVector3("WallCollisionPos5") + MODEL_CENTER_DIFF,   GetParameter("WallCollisionRadius5"),    GetParameter("WallCollisionHeight5")    },
        { GetParameterToVector3("WallCollisionPos6") + MODEL_CENTER_DIFF,   GetParameter("WallCollisionRadius6"),    GetParameter("WallCollisionHeight6")    },
        { GetParameterToVector3("WallCollisionPos7") + MODEL_CENTER_DIFF,   GetParameter("WallCollisionRadius7"),    GetParameter("WallCollisionHeight7")    },
        { GetParameterToVector3("WallCollisionPos8") + MODEL_CENTER_DIFF,   GetParameter("WallCollisionRadius8"),    GetParameter("WallCollisionHeight8")    },
        { GetParameterToVector3("WallCollisionPos9") + MODEL_CENTER_DIFF,   GetParameter("WallCollisionRadius9"),    GetParameter("WallCollisionHeight9")    },
        { GetParameterToVector3("WallCollisionPos10") + MODEL_CENTER_DIFF,  GetParameter("WallCollisionRadius10"),   GetParameter("WallCollisionHeight10")   },
        { GetParameterToVector3("WallCollisionPos11") + MODEL_CENTER_DIFF,  GetParameter("WallCollisionRadius11"),   GetParameter("WallCollisionHeight11")   },
        { GetParameterToVector3("WallCollisionPos12") + MODEL_CENTER_DIFF,  GetParameter("WallCollisionRadius12"),   GetParameter("WallCollisionHeight12")   },
        { GetParameterToVector3("WallCollisionPos13") + MODEL_CENTER_DIFF,  GetParameter("WallCollisionRadius13"),   GetParameter("WallCollisionHeight13")   },
        { GetParameterToVector3("WallCollisionPos14") + MODEL_CENTER_DIFF,  GetParameter("WallCollisionRadius14"),   GetParameter("WallCollisionHeight14")   },
        { GetParameterToVector3("WallCollisionPos15") + MODEL_CENTER_DIFF,  GetParameter("WallCollisionRadius15"),   GetParameter("WallCollisionHeight15")   },
        { GetParameterToVector3("WallCollisionPos16") + MODEL_CENTER_DIFF,  GetParameter("WallCollisionRadius16"),   GetParameter("WallCollisionHeight16")   },
    };

#pragma endregion
            
public:

    TomatoBossStage();              // コンストラクタ
    ~TomatoBossStage() = default;   // デストラクタ

    void Load(void)override;    // 読み込み

private:

    void ColliderLoad();    // 当たり判定の読み込み

};