#pragma once

#include "../../PlayerBase.h"

#include "../../../../../Common/Vector2.h"

class PlayerTripleAttackCollOperator;
class PlayerSimpleAttackCollOperator;

class OrangePlayer : public PlayerBase
{
public:
	OrangePlayer(MSG_SENDER_ID controlSenderId);
	~OrangePlayer()override = default;

	void PlayerLoad(void)override;

	// “–‚½‚è”»’èˆ—
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;
private:

#pragma region ’è”’è‹`

	// ƒAƒjƒ[ƒVƒ‡ƒ“``````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìí—Ş
	enum class ANIME_TYPE
	{
		NONE = -1,

		IDLE,
		WALK,
		RUN,
		PUNCH1,
		PUNCH2,
		PUNCH3,
		KICK,
		DODGE,
		DAMAGE,
		DEATH,

		MAX
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv‚²‚Æ‚ÌÄ¶‘¬“x
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::MAX] = 
	{
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "WalkAnimeSpeed"),	// WALK
		GetParameter("Anime", "RunAnimeSpeed"),		// RUN
		GetParameter("Anime", "Punch1AnimeSpeed"),	// PUNCH1
		GetParameter("Anime", "Punch2AnimeSpeed"),	// PUNCH2
		GetParameter("Anime", "Punch3AnimeSpeed"),	// PUNCH3
		GetParameter("Anime", "KickAnimeSpeed"),	// KICK
		GetParameter("Anime", "DodgeAnimeSpeed"),	// DODGE
		GetParameter("Anime", "DamageAnimeSpeed"),	// DAMAGE
		GetParameter("Anime", "DeathAnimeSpeed"),	// DEATH
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌƒtƒHƒ‹ƒ_‚Ü‚Å‚ÌƒpƒX
	const std::string ANIME_FOLDER_PATH = "Data/Model/Character/Orange/Anime/";

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv‚²‚Æ‚ÌƒpƒX
	const std::string ANIME_PATH_TABLE[(int)ANIME_TYPE::MAX] =
	{
		ANIME_FOLDER_PATH + "Idle.mv1",		// IDLE
		ANIME_FOLDER_PATH + "Walk.mv1",		// WALK
		ANIME_FOLDER_PATH + "Run.mv1",		// RUN
		ANIME_FOLDER_PATH + "Punch1.mv1",	// PUNCH1
		ANIME_FOLDER_PATH + "Punch2.mv1",	// PUNCH2
		ANIME_FOLDER_PATH + "Punch3.mv1",	// PUNCH3
		ANIME_FOLDER_PATH + "Kick.mv1",		// KICK
		ANIME_FOLDER_PATH + "Dodge.mv1",	// DODGE
		ANIME_FOLDER_PATH + "Hit.mv1",		// DAMAGE
		ANIME_FOLDER_PATH + "Death.mv1"		// DEATH
	};

	// ``````````````````````````ƒAƒjƒ[ƒVƒ‡ƒ“

#pragma endregion ’è”’è‹`

	// O’iUŒ‚‚Ì“–‚½‚è”»’èŠÇ—ƒNƒ‰ƒX‚ÌQÆ
	PlayerTripleAttackCollOperator* tripleAttackCollOperatorRef;

	// R‚èUŒ‚‚Ì“–‚½‚è”»’èŠÇ—ƒNƒ‰ƒX‚ÌQÆ
	PlayerSimpleAttackCollOperator* kickCollOperatorRef;

	// ‰ñ”ğ’†‚Ì–³“G”»’è
	bool isInvincible;
};