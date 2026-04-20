#pragma once

#include "../../CharactorBase.h"

#include "../CommonPlayerState/TripleAttack/PlayerTripleAttackStDefine.h"

class PlayerTripleAttackCollOperator;

class OrangePlayer : public CharactorBase
{
public:
	OrangePlayer();
	~OrangePlayer()override = default;

	void Load(void)override;
	void UiDraw(void)override;

private:

#pragma region ’è”’è‹`

	// ƒ‚ƒfƒ‹``````````````````````````````
	
	// ƒXƒP[ƒ‹
	const Vector3 MODEL_SCALE = GetParameterToVector3("ModelScale");

	// ƒTƒCƒY
	const Vector3 MODEL_SIZE = GetParameterToVector3("ModelSize") * MODEL_SCALE;

	// ’†S“_‚ÌƒYƒŒ
	const Vector3 MODEL_CENTER_DIFF = GetParameterToVector3("ModelCenterDiff") * MODEL_SCALE;

	// Šp“x‚ÌƒYƒŒ
	const Vector3 MODEL_LOCAL_ROT = GetParameterToVector3("ModelLocalRot") * (DX_PI_F / 180.0f);

	// `````````````````````````````````


	// “–‚½‚è”»’èî•ñ`````````````````````````````````````````

	// å‚ÉÚ’n”»’è‚Ég—p‚·‚éü•ªƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹n“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 LINE_COLLIDER_START_POS = Vector3();
	// å‚ÉÚ’n”»’è‚Ég—p‚·‚éü•ªƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹I“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 LINE_COLLIDER_END_POS = -Vector3::Yonly(MODEL_SIZE.y * 0.5f);
	// å‚ÉÚ’n”»’è‚Ég—p‚·‚éü•ªƒRƒ‰ƒCƒ_[‚Ìâ‘Î‚É“–‚½‚ç‚È‚¢‚¨‚¨‚æ‚»‚Ì‹——£
	const float LINE_COLLIDER_ENOUGH_DISTANCE = LINE_COLLIDER_END_POS.Length();

	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ì”¼Œa
	const float CAPSULE_COLLIDER_RADIUS = (MODEL_SIZE.x * 0.5f) * GetParameter("ModelToColliderRate");
	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹n“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_START_POS =
		Vector3::Yonly(
			(MODEL_SIZE.y * 0.5f) * GetParameter("ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);
	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹I“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_END_POS =
		-Vector3::Yonly(
			(MODEL_SIZE.y * 0.5f) * GetParameter("ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
			- GetParameter("ClimbOverHeight")
		);
	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìâ‘Î‚É“–‚½‚ç‚È‚¢‚¨‚¨‚æ‚»‚Ì‹——£
	const float CAPSULE_COLLIDER_ENOUGH_DISTANCE =
		(CAPSULE_COLLIDER_START_POS - CAPSULE_COLLIDER_END_POS).Length() 
		+ CAPSULE_COLLIDER_RADIUS;


	// ‰Ÿ‚µo‚µ‚ğs‚¤Û‚Ìd‚³
	const unsigned char COLLISION_PUSH_WEIGHT = (unsigned char)GetParameterToInt("CollisionPushWeight");
	// ````````````````````````````````````````````````


	// •Ï”‰Šú‰»Œn``````````````````````````

	// ‰ŠúÀ•W
	const Vector3 INIT_POS = GetParameterToVector3("InitPos");

	// ````````````````````````````````

	// ó‘Ô‚Ìí—Ş
	enum class STATE
	{
		NONE = -1,

		MOVE,			// ˆÚ“®ó‘Ô‚ğ’Ç‰Á
		SKILL_1,		// ƒXƒLƒ‹1iŠî–{“I‚Éu’ÊíUŒ‚vj
		SKILL_2,		// ƒXƒLƒ‹2iƒLƒƒƒ‰‚²‚Æ‚Ìu“Áê‹Zvj
		SKILL_3,		// ƒXƒLƒ‹3iƒLƒƒƒ‰‚²‚Æ‚Ìu“Áê‹Zvj
		SPECIAL_SKILL,	// ƒXƒyƒVƒƒƒ‹ƒXƒLƒ‹iƒLƒƒƒ‰‚²‚Æ‚Ìu•KE‹Zvj

		MAX
	};

	// ˆÚ“®ó‘Ô`````````````````````````````

	// ‰ÁZˆÚ“®—Ê
	const float MOVE_SPEED = GetParameter("MoveSpeed");

	// Å‘åˆÚ“®—Ê
	const float MOVE_SPEED_MAX = GetParameter("MoveSpeedMax");

	// ƒ_ƒbƒVƒ…‚ÌˆÚ“®—Ê”{—¦
	const float DASH_SPEED_RATE = GetParameter("DashSpeedRate");

	// ƒ_ƒbƒVƒ…‚ÌƒXƒ^ƒ~ƒi‚ÌÅ‘å—Êi1ƒtƒŒ[ƒ€‚¸‚ÂƒfƒNƒŠƒƒ“ƒgj
	const short DASH_STAMINA_MAX = (short)GetParameterToInt("DashStaminaMax");

	// `````````````````````````````````


	// ƒXƒLƒ‹1ó‘Ô```````````````````````````

	// UŒ‚‘ÎÛ‚Ì’Tõ”ÍˆÍ
	const float SKILL_1_TARGET_SERCH_RANGE = GetParameter("Skill1TargetSerchRange");

	// “–‚½‚è”»’è‚Ìƒ^ƒOƒe[ƒuƒ‹
	const TAG SKILL_1_COLL_TAG_TABLE[(int)PLAYER_TRIPLE_ATTACK_STAGE::MAX] =
	{
		TAG::ORANGE_PLAYER_TRIPLE_ATTACK_1,
		TAG::ORANGE_PLAYER_TRIPLE_ATTACK_2,
		TAG::ORANGE_PLAYER_TRIPLE_ATTACK_3,
	};

	// “–‚½‚è”»’è‚ÌƒTƒCƒYƒe[ƒuƒ‹i”¼Œaj
	const float SKILL_1_COLL_SIZE_TABLE[(int)PLAYER_TRIPLE_ATTACK_STAGE::MAX] =
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
	const float SKILL_1_COLL_START_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::MAX] =
	{
		GetParameter("Skill1CollStartTime1"),
		GetParameter("Skill1CollStartTime2"),
		GetParameter("Skill1CollStartTime3"),
	};
	// UŒ‚‚Ì”»’è‚ğ”­¶‚³‚¹‚éI—¹ŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const float SKILL_1_COLL_END_TIME[(int)PLAYER_TRIPLE_ATTACK_STAGE::MAX] =
	{
		GetParameter("Skill1CollEndTime1"),
		GetParameter("Skill1CollEndTime2"),
		GetParameter("Skill1CollEndTime3"),
	};

	// UŒ‚’†‚ÌˆÚ“®‘¬“x
	const float SKILL_1_ATTACK_MOVE_SPEED = GetParameter("Skill1AttackMoveSpeed");


	// `````````````````````````````````


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
	};

	// `````````````````````````````````

#pragma endregion ’è”’è‹`


	// •ø‚¦‚é‰ºˆÊƒNƒ‰ƒX‚ğŠi”[‚·‚é”z—ñ
	std::vector<ActorBase*> subObjArray;
	// •ø‚¦‚é‰ºˆÊƒNƒ‰ƒX‚ğŠi”[‚·‚é”z—ñ‚Ì’†‚©‚ç“Á’è‚ÌƒIƒuƒWƒFƒNƒg‚ğ’T‚·
	template<typename SubClass = ActorBase>
	SubClass* SubObjSerch(void) {
		for (ActorBase* obj : subObjArray) {
			if (dynamic_cast<SubClass*>(obj)) { return dynamic_cast<SubClass*>(obj); }
		}
		return nullptr;
	}

	// ƒƒCƒ“ˆ—``````````````
	void CharactorInit(void)override;
	void CharactorUpdate(void)override;
	void CharactorDraw(void)override;
	void CharactorAlphaDraw(void)override;
	void CharactorRelease(void)override;
	// ```````````````````
};