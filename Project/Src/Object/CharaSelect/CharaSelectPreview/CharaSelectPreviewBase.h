#pragma once

#include "../../../Manager/Net/NetWorkDefine.h"

#include "../../ActorBase.h"

class AnimationController;

class CharaSelectPreviewBase : public ActorBase
{
public:
	CharaSelectPreviewBase(MSG_SENDER_ID operatorSenderId, const Vector3& pos);
	virtual ~CharaSelectPreviewBase()override = default;

	// å—vŠÖ”Ä’è‹``````````
	void ReceptionUpdate(void)override;
	void Update(void)override;
	void SendUpdate(void)override;
	void UiDraw(void)override;
	// ````````````````

	// ©•ª‚ªV‚½‚É‘I‘ğ‚³‚ê‚½‚Æ‚«‚Ìˆ—
	void Select(void);

private:
	// ©g‚Ì‘€ìŒ ‚ğ‚Â‘—MÒID
	const MSG_SENDER_ID operatorSenderId;

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