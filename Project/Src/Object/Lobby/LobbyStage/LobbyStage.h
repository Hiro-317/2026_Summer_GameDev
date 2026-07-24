#pragma once

#include "../../ActorBase.h"

class LobbyStage : public ActorBase
{
public:
	LobbyStage() : ActorBase() {}
	~LobbyStage() = default;

	void Load(void)override {
		// ƒ‚ƒfƒ‹‚Ìƒ[ƒh
		trans.Load("Lobby/Lobby");
		trans.scale = 10.0f;
		trans.pos = Vector3();
	}
	void Update(void)override {}
};