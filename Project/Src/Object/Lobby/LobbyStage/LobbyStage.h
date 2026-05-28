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
		trans.scale = 1.0f;
		trans.pos = Vector3::Zonly(400.0f);
	}
	void Update(void)override {}
};