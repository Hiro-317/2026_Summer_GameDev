#pragma once

#include "../CharaSelectObj.h"

class TanakaSelect : public CharaSelectObj
{
public:

	TanakaSelect();
	~TanakaSelect()override = default;

	void Load(void)override;

private:

	// •Ï”‰Šú‰»Œn``````````````````````````

	// ƒXƒP[ƒ‹
	const Vector3 MODEL_SCALE = GetParameterToVector3("ModelScale2");

	// ƒTƒCƒY
	const Vector3 MODEL_SIZE = GetParameterToVector3("ModelSize2") * MODEL_SCALE;

	// ’†S“_‚ÌƒYƒŒ
	const Vector3 MODEL_CENTER_DIFF = GetParameterToVector3("ModelCenterDiff") * MODEL_SCALE;

	// ````````````````````````````````

};