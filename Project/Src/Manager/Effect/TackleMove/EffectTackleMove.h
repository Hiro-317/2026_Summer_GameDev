#pragma once

#include "../EffectBase.h"

class EffectTackleMove : public EffectBase {
public:

	EffectTackleMove(const ParameterLoad& parameter, EFFECT_NAME name, const Transform* follow = nullptr, const Vector3& local = Vector3(),
	const bool followRotX = true, const bool followRotY = true, const bool followRotZ = true);

	~EffectTackleMove() = default;
};