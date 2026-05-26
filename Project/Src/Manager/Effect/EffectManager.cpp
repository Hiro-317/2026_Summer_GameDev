#include "../../pch.h"

#include "EffectManager.h"

EffectManager*  EffectManager::ins = nullptr;

EffectManager::EffectManager(void)
{
	parameter = new ParameterLoad("Data/Parameter/Effect/");
}

EffectManager::~EffectManager()
{
	delete parameter;
}

void EffectManager::Update(void) {

	for (auto info = effectInfo.begin(); info != effectInfo.end();) {
		(*info)->Update();
		if ((*info)->IsEnd()) { (*info)->Release(); info = effectInfo.erase(info); delete (*info); }
		else { info++; }
	}
}

void EffectManager::CreateEffect(EFFECT_NAME name, const Transform* follow, const Vector3& local) {

	effectInfo.emplace_back(EffectFactory::CreateEffect(*parameter, name, follow, local));
	effectInfo.back()->Load();
}

void EffectManager::CreateEffect(EFFECT_NAME name, const Transform& trans) {

	effectInfo.emplace_back(EffectFactory::CreateEffect(*parameter, name, trans));
	effectInfo.back()->Load();
}

void EffectManager::StopEffect(EFFECT_NAME name) {

	for (auto info = effectInfo.begin(); info != effectInfo.end();) {
		if ((*info)->GetName() == name) { (*info)->StopEffect(); }
		else { info++; }
	}
}