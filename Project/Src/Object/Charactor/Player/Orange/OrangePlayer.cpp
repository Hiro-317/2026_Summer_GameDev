#include "OrangePlayer.h"

#include "../CommonPlayerState/Move/PlayerMoveState.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"

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

#pragma endregion


#pragma region “–‚½‚è”»’èî•ñİ’è

	ColliderCreate(new LineCollider(TAG::PLAYER, Vector3(), Vector3::Yonly(-MODEL_SIZE.y * 0.5f)));

#pragma endregion


#pragma region ó‘Ôİ’è

	// ˆÚ“®ó‘Ô‚ğ’Ç‰Á‚·‚é
	AddState(
		(int)STATE::MOVE,
		new PlayerMoveState(
			// ©•ª‚Ìó‘Ô‚É‘JˆÚ‚·‚éŠÖ”
			[&]() { state = (int)STATE::MOVE; },
			// ©•ª‚Ìó‘Ô‚©‚Ç‚¤‚©‚ğ•Ô‚·ŠÖ”
			[&]() { return state == (int)STATE::MOVE; },
			// QÆiˆÚ“®—Ê / ‰¡²‰Á‘¬“x‚ÌÅ‘å’l / Šp“xj
			accelSum, ACCEL_MAX,trans.angle,
			// ’è”i‰ÁZˆÚ“®—Ê / ˆÚ“®—Ê‚ÌÅ‘å’l / ƒ_ƒbƒVƒ…‚ÌˆÚ“®—Ê”{—¦ / ƒXƒ^ƒ~ƒi—Êj
			MOVE_SPEED, MOVE_SPEED_MAX, DASH_SPEED_RATE, DASH_STAMINA_MAX,
			// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶ŠÖ”‚Ìƒ|ƒCƒ“ƒ^i‘Ò‹@ / •à‚« / ‘–‚èj
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE); },
			[&]() { AnimePlay((int)ANIME_TYPE::WALK); },
			[&]() { AnimePlay((int)ANIME_TYPE::RUN); }
		)
	);

#pragma endregion
}

void OrangePlayer::CharactorInit(void)
{
	SetGravityFlg(true);
	// ‰Šúó‘Ô‚ğˆÚ“®ó‘Ô‚É‚·‚é
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
