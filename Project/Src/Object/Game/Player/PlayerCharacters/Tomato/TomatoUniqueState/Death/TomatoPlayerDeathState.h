#pragma once

#include "../../../../../../Common/CharacterBase/CharacterStateBase.h"

struct Transform;
struct Vector3;

class TomatoPlayerDeathState : public CharacterStateBase
{
public:
    TomatoPlayerDeathState(
        Transform& playerTrans,
        Vector3& velocity,
        std::function<void(bool)> setIsGravity,
        std::function<void(bool)> setJudge,
        std::function<void(void)> playerDeathSetting);
    ~TomatoPlayerDeathState() override = default;

    void Enter(void) override;
    void Update(void) override;
    void Exit(void) override;

private:
    Transform& playerTrans;
    Vector3& velocity;
    const std::function<void(bool)> setIsGravity;
    const std::function<void(bool)> setJudge;
    const std::function<void(void)> playerDeathSetting;

    static constexpr float HOP_HEIGHT = 60.0f;
    static constexpr float DEATH_TIME = 60.0f;
    float elapsedTime;
    float startHeight;
    float heading;
    bool finished;
};