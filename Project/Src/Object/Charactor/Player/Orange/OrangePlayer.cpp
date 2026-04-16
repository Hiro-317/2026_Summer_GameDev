#include "OrangePlayer.h"

OrangePlayer::OrangePlayer() :
	CharactorBase("Data/Parameter/Charactor/Player/Orange/OrangePlayerParameter.csv")
{
}

void OrangePlayer::Load(void)
{

#pragma region ƒ‚ƒfƒ‹

	// ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
	trans.Load("Data/Model/Charactor/Orange/OrangePlayer.mv1");

	// ƒ‚ƒfƒ‹‚ÌƒXƒP[ƒ‹‚ğİ’è
	trans.scale = MODEL_SCALE;

	// ƒAƒjƒ[ƒVƒ‡ƒ“````````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚ğ¶¬‚·‚é
	CreateAnimationController();

	// ŠeíƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	for (int i = 0; i < (int)ANIME_TYPE::MAX; i++) {
		AddAnimation(i, ANIME_SPEED_TABLE[i], ANIME_PATH_TABLE[i].c_str());
	}

	// ```````````````````````````````````

#pragma endregion

}

void OrangePlayer::CharactorInit(void)
{
	SetGravityFlg(false);
}

void OrangePlayer::CharactorUpdate(void)
{
}

void OrangePlayer::CharactorDraw(void)
{
}

void OrangePlayer::CharactorAlphaDraw(void)
{
}

void OrangePlayer::CharactorRelease(void)
{
}
