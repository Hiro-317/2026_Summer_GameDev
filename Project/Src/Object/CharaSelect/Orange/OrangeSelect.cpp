#include "OrangeSelect.h"

OrangeSelect::OrangeSelect()
{
}

void OrangeSelect::Load(void)
{

	// ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
	trans.Load("Charactor/Orange/OrangePlayer");

	// ƒ‚ƒfƒ‹‚ÌƒXƒP[ƒ‹‚ğİ’è
	trans.scale = MODEL_SCALE;

	// ƒ‚ƒfƒ‹‚Ì’†S“_‚ÌƒYƒŒ
	trans.centerDiff = MODEL_CENTER_DIFF;

	// Šp“x‚ÌƒYƒŒ(ƒ[ƒJƒ‹‰ñ“])
	trans.localAngle = MODEL_LOCAL_ROT;

	// ƒAƒjƒ[ƒVƒ‡ƒ“````````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚ğ¶¬‚·‚é
	CreateAnimationController();

	// ŠeíƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	for (int i = 0; i < (int)ANIME_TYPE::MAX; i++) {
		AddAnimation(i, ANIME_SPEED_TABLE[i], ANIME_PATH_TABLE[i].c_str());
	}

	// ```````````````````````````````````
}

void OrangeSelect::CharactorInit(void)
{
}

void OrangeSelect::CharactorUpdate(void)
{
}

void OrangeSelect::CharactorDraw(void)
{
}

void OrangeSelect::CharactorAlphaDraw(void)
{
}

void OrangeSelect::CharactorRelease(void)
{
}
