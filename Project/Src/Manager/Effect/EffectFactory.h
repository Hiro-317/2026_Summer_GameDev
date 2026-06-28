#pragma once

#include "EffectBase.h"
#include "TackleMove/EffectTackleMove.h"

class EffectFactory {

public:

	static EffectBase* CreateEffect(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* trans,
	bool followRotX, bool followRotY, bool followRotZ) {

		EffectBase* res{};

		switch (name){
		case EFFECT_NAME::TACKLE_MOVE: { res = new EffectTackleMove(parameter, name, local, trans, followRotX, followRotY, followRotZ); break; }
		default: { break; }
		}
		return res;
	}
};