#pragma once
#include "../../../../ActorBase.h"


class GrapeBossWeaponBase : public ActorBase
{
public:

	GrapeBossWeaponBase(std::string parameterPath, int model);
	~GrapeBossWeaponBase();

	virtual void Load(void) {};

};
