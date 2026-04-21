#pragma once

#include "../CharaSelectObj.h"

class OrangeSelect : public CharaSelectObj
{
public:

	OrangeSelect();
	~OrangeSelect()override = default;

	void Load(void)override;

private:

	// ïœêîèâä˙âªånÅ`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`

	// ÉXÉPÅ[Éã
	const Vector3 MODEL_SCALE = GetParameterToVector3("ModelScale1");

	// ÉTÉCÉY
	const Vector3 MODEL_SIZE = GetParameterToVector3("ModelSize1") * MODEL_SCALE;

	// íÜêSì_ÇÃÉYÉå
	const Vector3 MODEL_CENTER_DIFF = GetParameterToVector3("ModelCenterDiff") * MODEL_SCALE;

	// Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`

};