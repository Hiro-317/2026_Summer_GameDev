#include "EffectTackleMove.h"


EffectTackleMove::EffectTackleMove(const ParameterLoad& parameter, EFFECT_NAME name, const Transform* follow, const Vector3& local,
	const bool followRotX, const bool followRotY, const bool followRotZ)
	: EffectBase(parameter)
{
	info.follow = follow;
	info.name = name;
	info.trans.pos = local;

	info.followRotX = followRotX;
	info.followRotY = followRotY;
	info.followRotZ = followRotZ;

	info.trans.LoadEffect("TackleMove");

	info.trans.angle = GetParameterToVector3("TackleMove", "Angle");
	info.trans.scale = GetParameterToVector3("TackleMove", "Scale");
}