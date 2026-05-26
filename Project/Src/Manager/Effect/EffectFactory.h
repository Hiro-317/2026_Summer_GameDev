#pragma once

#include "EffectBase.h"
#include "Boom/EffectBoom.h"

class EffectFactory {

public:

	static EffectBase* CreateEffect(EFFECT_NAME name) {
		EffectBase* res{};

		switch (name){
		case EFFECT_NAME::BOOM: { res = new EffectBoom(); break; }
		default: { break; }
		}
		return res;
	}
};