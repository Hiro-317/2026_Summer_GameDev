#include "GrapeBossWeaponBase.h"


GrapeBossWeaponBase::GrapeBossWeaponBase(std::string parameterPath, int model) : ActorBase(parameterPath)
{
	trans.Duplicate(model);
}

GrapeBossWeaponBase::~GrapeBossWeaponBase()
{
}
