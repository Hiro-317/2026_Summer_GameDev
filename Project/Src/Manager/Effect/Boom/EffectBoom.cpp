#include "../../../pch.h"

#include "EffectBoom.h"

EffectBoom::EffectBoom()
{
}

EffectBoom::~EffectBoom()
{
}

void EffectBoom::Load(void)
{
	info.trans.LoadEffect("ikuna_state");

	info.trans.pos = pos;
	info.trans.angle = ANGLE;
	info.trans.scale = SCALE;
	info.speed = SPEED;
	
	playHandle = PlayEffekseer3DEffect(info.trans.model);
}
