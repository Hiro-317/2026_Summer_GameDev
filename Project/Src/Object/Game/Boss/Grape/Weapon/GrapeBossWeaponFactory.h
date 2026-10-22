#pragma once
#include "GrapeBossWeaponManager.h"

class GrapeBossWeaponFactory {

public:

	// インスタンスの生産
	static BombType Create(int model, int type, int front, int back);
};