#include "../../pch.h"

#include "EffectManager.h"


EffectManager::EffectManager(const Vector3* followPos) : followPos(followPos)
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::Update(void) {
	
	for (auto& info : effectInfo) {
		info->Update(followPos);
	}
}

void EffectManager::CreateEffect(EFFECT_NAME name) {

	effectInfo.emplace_back(EffectFactory::CreateEffect(name));
	effectInfo.back()->Load();
}
