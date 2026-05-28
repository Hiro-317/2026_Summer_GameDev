#include "Lobby.h"

Lobby::Lobby() : 
	ActorBase()
{
}

void Lobby::Load(void)
{
	// ƒ‚ƒfƒ‹‚Ìƒ[ƒh
	trans.Load("Lobby/Lobby");
	trans.scale = 0.05f;
	trans.pos = Vector3::YZonly(-200.0f, 400.0f);
}