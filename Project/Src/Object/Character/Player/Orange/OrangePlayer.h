#pragma once

#include "../PlayerBase.h"

class PlayerTripleAttackCollOperator;


class OrangePlayer : public PlayerBase
{
public:
	OrangePlayer(MSG_SENDER_ID operatorSenderId);
	~OrangePlayer()override = default;

	void PlayerLoad(void)override;

	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;


private:

#pragma region ’è”’è‹`

	// ƒXƒLƒ‹1ó‘Ô```````````````````````````

	// UŒ‚‘ÎÛ‚Ì’Tõ”ÍˆÍ
	const float SKILL_1_TARGET_SERCH_RANGE = GetParameter("Skill1TargetSerchRange");

	// “–‚½‚è”»’è‚Ìƒ^ƒOƒe[ƒuƒ‹
	const std::array<COLLIDER_TAG, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> SKILL_1_COLL_TAG_TABLE =
	{
		COLLIDER_TAG::ORANGE_PLAYER_TRIPLE_ATTACK_1,
		COLLIDER_TAG::ORANGE_PLAYER_TRIPLE_ATTACK_2,
		COLLIDER_TAG::ORANGE_PLAYER_TRIPLE_ATTACK_3,
	};

	// “–‚½‚è”»’è‚ÌƒTƒCƒYƒe[ƒuƒ‹i”¼Œaj
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> SKILL_1_COLL_SIZE_TABLE =
	{
		GetParameter("Skill1CollSize1"),
		GetParameter("Skill1CollSize2"),
		GetParameter("Skill1CollSize3"),
	};

	// “–‚½‚è”»’è‚ÌƒIƒtƒZƒbƒg
	const Vector3 SKILL_1_COLL_LOCAL_POS = GetParameterToVector3("Skill1CollLocalPos");

	// UŒ‚‚»‚Ì‚à‚Ì‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int SKILL_1_COOL_TIME = GetParameterToInt("Skill1CoolTime");

	// UŒ‚‚ªŸ’i‚ÉŒq‚ª‚é—P—\ŠÔ
	const int SKILL_1_ATTACK_NEXT_STAGE_CONTINUE_TIME = GetParameterToInt("Skill1AttackNextStageContinueTime");

	// UŒ‚‚Ì”»’è‚ğ”­¶‚³‚¹‚éŠJnŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> SKILL_1_COLL_START_TIME =
	{
		GetParameter("Skill1CollStartTime1"),
		GetParameter("Skill1CollStartTime2"),
		GetParameter("Skill1CollStartTime3"),
	};
	// UŒ‚‚Ì”»’è‚ğ”­¶‚³‚¹‚éI—¹ŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> SKILL_1_COLL_END_TIME =
	{
		GetParameter("Skill1CollEndTime1"),
		GetParameter("Skill1CollEndTime2"),
		GetParameter("Skill1CollEndTime3"),
	};

	// UŒ‚’†‚ÌˆÚ“®‘¬“x
	const float SKILL_1_ATTACK_MOVE_SPEED = GetParameter("Skill1AttackMoveSpeed");

	// ƒXƒLƒ‹‚ÌUŒ‚”{—¦
	const short SKILL_1_ATTACK_RATE_PERCENT = GetParameter("Skill1AttackRatePercent");

	// ```````````````````````````ƒXƒLƒ‹1ó‘Ô


	// ƒXƒLƒ‹2ó‘Ô```````````````````````````

	// UŒ‚‘ÎÛ‚Ì’Tõ”ÍˆÍ
	const float SKILL_2_TARGET_SERCH_RANGE = GetParameter("Skill2TargetSerchRange");

	// “–‚½‚è”»’è‚Ìƒ^ƒO
	const COLLIDER_TAG SKILL_2_COLL_TAG = COLLIDER_TAG::ORANGE_PLAYER_KICK_ATTACK;

	// “–‚½‚è”»’è‚ÌƒTƒCƒYi”¼Œaj
	const float SKILL_2_COLL_SIZE_TABLE = GetParameter("Skill2CollSize");

	// “–‚½‚è”»’è‚ÌƒIƒtƒZƒbƒg
	const Vector3 SKILL_2_COLL_LOCAL_POS = GetParameterToVector3("Skill2CollLocalPos");

	// UŒ‚‚»‚Ì‚à‚Ì‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int SKILL_2_COOL_TIME = GetParameterToInt("Skill2CoolTime");

	// UŒ‚‚Ì”»’è‚ğ”­¶‚³‚¹‚éŠJnŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const float SKILL_2_COLL_START_TIME = GetParameter("Skill2CollStartTime");

	// UŒ‚‚Ì”»’è‚ğ”­¶‚³‚¹‚éI—¹ŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const float SKILL_2_COLL_END_TIME = GetParameter("Skill2CollEndTime");

	// UŒ‚’†‚ÌˆÚ“®‘¬“x
	const float SKILL_2_ATTACK_MOVE_SPEED = GetParameter("Skill2AttackMoveSpeed");

	// ƒXƒLƒ‹‚ÌUŒ‚”{—¦
	const short SKILL_2_ATTACK_RATE_PERCENT = GetParameter("Skill2AttackRatePercent");

	// ```````````````````````````ƒXƒLƒ‹2ó‘Ô


	// ƒXƒLƒ‹3ó‘Ô```````````````````````````

	// ‰ñ”ğ‚»‚Ì‚à‚Ì‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int SKILL_3_COOL_TIME = GetParameterToInt("Skill3CoolTime");

	// ‰ñ”ğ‚Ì–³“G”»’è‚ğ”­¶‚³‚¹‚éŠJnŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const float SKILL_3_INVI_START_TIME = GetParameter("Skill3InviStartTime");
	// ‰ñ”ğ‚Ì–³“G”»’è‚ğ”­¶‚³‚¹‚éI—¹ŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const float SKILL_3_INVI_END_TIME = GetParameter("Skill3InviEndTime");

	// ‰ñ”ğ’†‚ÌˆÚ“®‘¬“x
	const float SKILL_3_MOVE_SPEED = GetParameter("Skill3DodgeMoveSpeed");

	// ```````````````````````````ƒXƒLƒ‹2ó‘Ô



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
		GetParameter("IdleAnimeSpeed"),		// IDLE
		GetParameter("WalkAnimeSpeed"),		// WALK
		GetParameter("RunAnimeSpeed"),		// RUN
		GetParameter("Punch1AnimeSpeed"),	// PUNCH1
		GetParameter("Punch2AnimeSpeed"),	// PUNCH2
		GetParameter("Punch3AnimeSpeed"),	// PUNCH3
		GetParameter("KickAnimeSpeed"),		// KICK
		GetParameter("DodgeAnimeSpeed"),	// DODGE
		GetParameter("DamageAnimeSpeed"),	// DAMAGE
		GetParameter("DeathAnimeSpeed"),	// DEATH
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌƒtƒHƒ‹ƒ_‚Ü‚Å‚ÌƒpƒX
	const std::string ANIME_FOLDER_PATH = "Data/Model/Charactor/Orange/Anime/";

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

};