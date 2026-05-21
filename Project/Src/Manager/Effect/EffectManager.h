#pragma once

#include <vector>

#include "EffectFactory.h"

class EffectManager {
public:

	EffectManager(const Vector3* followPos);
	~EffectManager();

	void Update(void);

	void CreateEffect(EFFECT_NAME name);

private:

	std::vector<EffectBase*> effectInfo;

	const Vector3* followPos;
};