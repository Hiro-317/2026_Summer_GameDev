#include "../../pch.h"

#include "EffectManager.h"


EffectManager::EffectManager(const Vector3* followPos = nullptr) : followPos(followPos)
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::Update(void) {
	for (auto info = effectInfo.begin(); info != effectInfo.end();) {
		(*info)->Update(followPos);
		if ((*info)->IsEnd()) { (*info)->Release(); info = effectInfo.erase(info); }
		else { info++; }
	}
}

void EffectManager::CreateEffect(EFFECT_NAME name) {

	effectInfo.emplace_back(EffectFactory::CreateEffect(name));
	effectInfo.back()->Load();
}
