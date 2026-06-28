#pragma once

#include "EffectBase.h"
#include "TackleMove/EffectTackleMove.h"

class EffectFactory {

public:

	static EffectBase* CreateEffect(const ParameterLoad& parameter, EFFECT_NAME name, const Transform* trans = nullptr, const Vector3& local = Vector3(),
	const bool followRotX = true, const bool followRotY = true, const bool followRotZ = true) {

		EffectBase* res{};

		switch (name){
		case EFFECT_NAME::TACKLE_MOVE: { res = new EffectTackleMove(parameter, name, trans, local, followRotX, followRotY, followRotZ); break; }
		default: { break; }
		}
		return res;
	}
};