#pragma once

#include "../../ActorBase.h"

class AnimationController;

class LobbyCharaPreviewBase : public ActorBase
{
public:
	LobbyCharaPreviewBase(const Vector3& pos);
	virtual ~LobbyCharaPreviewBase()override = default;

	// XVˆ—Ä’è‹`
	void Update(void)override;

private:

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[
	AnimationController* anime;
	void AnimationControllerCreate(void);

	// å—vŠÖ”Ä’è‹``````````
	void SubInit(void)override;
	void SubRelease(void)override;
	// ````````````````

	// Ä¶‚·‚éƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv
	int playAnimeType;

protected:
	// ƒ‚ƒfƒ‹‚É‚­‚Á‚Â‚¢‚Ä‚éFBXƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	void AddInFbxAnimation(int inFbxMaxIndex, float speed, int playAnimeType);
	// •Êƒtƒ@ƒCƒ‹‚É•Û‘¶‚³‚ê‚Ä‚¢‚éFBXƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	void AddAnimation(const char* filePath, float speed, int index = 0);
};