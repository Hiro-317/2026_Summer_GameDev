#pragma once

#include "../StageBase.h"

class TomatoBossStage : public ActorBase
{
private:

    const Vector3 GROUND_COLLISION_SIZE = Vector3(10077.034f, 50.0f, 9234.112f);

    const Vector3 MODEL_CENTER_DIFF = Vector3::Yonly(-80.0f);

public:

    TomatoBossStage();
    ~TomatoBossStage() = default;

    void Load(void)override;
    //void Init(void);
    //void Update(void);
    //void Draw(void);
    //void Release(void);

private:


};