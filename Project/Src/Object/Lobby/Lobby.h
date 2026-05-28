#pragma once

#include "../ActorBase.h"

class Lobby : public ActorBase
{
public:
	Lobby();
	~Lobby() = default;

	void Load(void)override;
	void Update(void)override {}
};