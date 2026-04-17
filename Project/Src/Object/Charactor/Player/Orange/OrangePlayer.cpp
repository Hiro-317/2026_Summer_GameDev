#include "OrangePlayer.h"

#include "../PlayerCommon/State/Move/PlayerMoveState.h"

OrangePlayer::OrangePlayer() :
	CharactorBase("Data/Parameter/Charactor/Player/Orange/OrangePlayerParameter.csv")
{
}

void OrangePlayer::Load(void)
{

#pragma region ƒ‚ƒfƒ‹

	// ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
	trans.Load("Charactor/Orange/OrangePlayer");

	// ƒ‚ƒfƒ‹‚ÌƒXƒP[ƒ‹‚ğİ’è
	trans.scale = MODEL_SCALE;

	// Šp“x‚ÌƒYƒŒ(ƒ[ƒJƒ‹‰ñ“])
	trans.localAngle = LOCAL_ROT;

	// ƒAƒjƒ[ƒVƒ‡ƒ“````````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚ğ¶¬‚·‚é
	CreateAnimationController();

	// ŠeíƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	for (int i = 0; i < (int)ANIME_TYPE::MAX; i++) {
		AddAnimation(i, ANIME_SPEED_TABLE[i], ANIME_PATH_TABLE[i].c_str());
	}

	// ```````````````````````````````````

#pragma endregion

	AddState(
		(int)STATE::MOVE,
		new PlayerMoveState(
			// ©•ª‚Ìó‘Ô‚É‘JˆÚ‚·‚éŠÖ”
			[&]() { state = (int)STATE::MOVE; },
			// ©•ª‚Ìó‘Ô‚©‚Ç‚¤‚©‚ğ•Ô‚·ŠÖ”
			[&]() { return state == (int)STATE::MOVE; },
			// ˆÚ“®—Ê / Šp“x ‚ÌQÆ
			accelSum, trans.angle,
			// ’è”
			10.0f, 2.0f, 250,
			// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶ŠÖ”‚Ìƒ|ƒCƒ“ƒ^
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE); },
			[&]() { AnimePlay((int)ANIME_TYPE::WALK); },
			[&]() { AnimePlay((int)ANIME_TYPE::RUN); }
		)
	);

	ACCEL_MAX = 100.0f;

}

void OrangePlayer::CharactorInit(void)
{
	SetGravityFlg(false);
	state = (int)STATE::MOVE;
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
