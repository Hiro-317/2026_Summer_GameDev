#pragma once

#include "../../../../../../Common/CharacterBase/CharacterStateBase.h"

struct Quaternion;
struct Vector3;

class TomatoPlayerDamageState : public CharacterStateBase
{
public:
    TomatoPlayerDamageState(
        Quaternion& rotation,
        Vector3& velocity,
        std::function<void(void)> inviStart,
        std::function<void(void)> defaultChangeState);
    ~TomatoPlayerDamageState() override = default;

    void Enter(void) override;
    void Update(void) override;
    void Exit(void) override;

private:
    // 軽くのけぞってから元へ戻る演出を時間で管理する。
    static constexpr float DAMAGE_TIME = 40.0f;
    Quaternion& rotation;
    Vector3& velocity;
    const std::function<void(void)> inviStart;
    const std::function<void(void)> defaultChangeState;
    float elapsedTime;
    float heading;
};
