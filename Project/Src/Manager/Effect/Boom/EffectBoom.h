#pragma once

#include "../EffectBase.h"

class EffectBoom : public EffectBase {

public:

	EffectBoom();
	~EffectBoom();

	void Load(void) override;

private:
	Vector3 pos;
	Vector3 ANGLE;
	Vector3 SCALE;
	int SPEED;
};