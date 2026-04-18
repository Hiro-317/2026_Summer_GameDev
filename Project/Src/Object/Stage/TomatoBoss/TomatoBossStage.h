#pragma once

#include "../StageBase.h"

class TomatoBossStage : public StageBase
{
public:
    TomatoBossStage(int model);
    ~TomatoBossStage() = default;

    void Load(void) override {}
    void Init(void) override {}
    void Update(void) override {}
    void Draw(void) override {}
    void Release(void) override {}

private:

};