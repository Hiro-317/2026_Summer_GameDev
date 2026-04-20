#include "CharaSelectObj.h"

CharaSelectObj::CharaSelectObj(void) : ActorBase("Data/Parameter/Charactor/Player/Select/SelectPlayerParameter.csv")
{
}

void CharaSelectObj::Load(void)
{
	// ƒ‚ƒfƒ‹‚ÌƒXƒP[ƒ‹‚ğİ’è
	trans.scale = MODEL_SCALE;

	// ƒ‚ƒfƒ‹‚Ì’†S“_‚ÌƒYƒŒ
	trans.centerDiff = MODEL_CENTER_DIFF;

	// Šp“x‚ÌƒYƒŒ(ƒ[ƒJƒ‹‰ñ“])
	trans.localAngle = MODEL_LOCAL_ROT;

	// À•W
	trans.pos = INIT_POS;

	ActorBase::Init();

	// ƒAƒjƒ[ƒVƒ‡ƒ“````````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚ğ¶¬‚·‚é
	CreateAnimationController();

	// ŠeíƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	for (int i = 0; i < (int)ANIME_TYPE::MAX; i++) {
		AddAnimation(i, ANIME_SPEED_TABLE[i], ANIME_PATH_TABLE[i].c_str());
	}

	// ```````````````````````````````````
}

void CharaSelectObj::AddAnimation(int index, float speed, const char* filePath)
{
	anime->Add(index, speed, filePath);
}

void CharaSelectObj::SubUpdate(void)
{
	anime->Play(static_cast<int>(ANIME_TYPE::DANCE));
}
