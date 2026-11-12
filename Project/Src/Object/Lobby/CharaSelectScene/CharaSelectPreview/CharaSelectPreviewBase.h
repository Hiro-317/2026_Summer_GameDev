#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

#include "../../../../Manager/Net/NetWorkDefine.h"

class AnimationController;
class CameraBase;

class CharaSelectPreviewBase : public ActorBase
{
public:
	CharaSelectPreviewBase();
	CharaSelectPreviewBase(const std::string& parameterPath);
	virtual ~CharaSelectPreviewBase()override = default;

	// å—vŠÖ”Ä’è‹``````````
	void Load(void)override;
	void Update(void)override;
	void Draw(void)override;
	// ````````````````

private:

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[
	AnimationController* anime;
	void AnimationControllerCreate(void);

	// å—vŠÖ”Ä’è‹``````````
	void BaseInit(void)override;
	void BaseRelease(void)override;
	// ````````````````

	// Ä¶‚·‚éƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv
	int playAnimeType;

	int modelPreviewScreen;

	CameraBase* camera;

protected:

	virtual void SubLoad(void) = 0;

	std::string CHARA_NAME;
	// ƒpƒ‰ƒ[ƒ^[
	short HP;
	short ATTACK;
	short DEFENCE;
	short SPEED;

	int skill_1_image;
	int skill_2_image;
	int skill_3_image;

	/// <summary>
	/// ƒ‚ƒfƒ‹‚É‚­‚Á‚Â‚¢‚Ä‚éFBXƒAƒjƒ[ƒVƒ‡ƒ“‚ğ‘S•”“o˜^‚·‚é
	/// </summary>
	/// <param name="inFbxMaxIndex">‚­‚Á‚Â‚¢‚Ä‚éƒAƒjƒ[ƒVƒ‡ƒ“‚Ì”</param>
	/// <param name="speed">Ä¶‘¬“x</param>
	/// <param name="loop">ƒ‹[ƒvÄ¶ƒtƒ‰ƒO”z—ñiw’è‚È‚µ‚Å‘S‚Äƒ‹[ƒvÄ¶—LŒø‚Å“o˜^j</param>
	void AddInFbxAnimation(int inFbxMaxIndex, float speed, bool loop, int playAnimeType);

	/// <summary>
	/// •Êƒtƒ@ƒCƒ‹‚É•Û‘¶‚³‚ê‚Ä‚¢‚éFBXƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	/// </summary>
	/// <param name="index">QÆ”Ô†</param>
	/// <param name="speed">Ä¶‘¬“x</param>
	/// <param name="loop">ƒ‹[ƒvÄ¶ƒtƒ‰ƒO</param>
	/// <param name="filePath">ƒpƒX</param>
	void AddAnimation(int index, float speed, bool loop, const char* filePath);
};