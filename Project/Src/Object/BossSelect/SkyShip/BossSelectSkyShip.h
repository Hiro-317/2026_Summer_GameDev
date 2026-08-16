#pragma once

#include "../../Character/CharacterBase.h"

class BossSelectSkyShip : public CharacterBase
{
public:

	// èÛë‘
	enum class STATE {

		None = -1,

		Start,

		Move,

		Max
	};

	BossSelectSkyShip();

	~BossSelectSkyShip()override = default;

	// ì«Ç›çûÇ›
	void Load(void)override;
};