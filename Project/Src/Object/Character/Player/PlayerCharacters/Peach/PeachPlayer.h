#pragma once

#include "../../PlayerBase.h"

#include "../../../../../Common/Vector2.h"

class PeachPlayer : public PlayerBase
{
public:

	PeachPlayer(MSG_SENDER_ID operatorSenderId);
	~PeachPlayer()override = default;

	void PlayerLoad(void)override;

	// “–‚½‚è”»’èˆ—
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;

private:

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìí—Ş
	enum class ANIME_TYPE
	{
		NONE = -1,

		IDLE,
		WALK,
		RUN,
		DAMAGE,
		DEATH,
		PUNCH,
		HEAL,

		MAX
	};

	const float ANIME_SPEED[(int)ANIME_TYPE::MAX] = {
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		2.5f
	};

#pragma region ƒXƒLƒ‹1‚Ì’è”’è‹`

	// ƒXƒLƒ‹2ó‘Ô```````````````````````````

	// UŒ‚‘ÎÛ‚Ì’Tõ”ÍˆÍ
	const float SKILL_1_TARGET_SERCH_RANGE = GetParameter("SimpleAttack", "TargetSerchRange");

	// “–‚½‚è”»’è‚Ìƒ^ƒO
	const COLLIDER_TAG SKILL_1_COLL_TAG = COLLIDER_TAG::PLAYER_ATTACK;

	// “–‚½‚è”»’è‚ÌƒTƒCƒYi”¼Œaj
	const float SKILL_1_COLL_SIZE_TABLE = GetParameter("SimpleAttack", "CollSize");

	// “–‚½‚è”»’è‚ÌƒIƒtƒZƒbƒg
	const Vector3 SKILL_1_COLL_LOCAL_POS = GetParameterToVector3("SimpleAttack", "CollLocalPos");

	// UŒ‚‚»‚Ì‚à‚Ì‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int SKILL_1_COOL_TIME = GetParameterToInt("SimpleAttack", "CoolTime");

	// UŒ‚‚Ì”»’è‚ğ”­¶‚³‚¹‚éŠJnŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const float SKILL_1_COLL_START_TIME = GetParameter("SimpleAttack", "CollStartTime");

	// UŒ‚‚Ì”»’è‚ğ”­¶‚³‚¹‚éI—¹ŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const float SKILL_1_COLL_END_TIME = GetParameter("SimpleAttack", "CollEndTime");

	// UŒ‚’†‚ÌˆÚ“®‘¬“x
	const float SKILL_1_ATTACK_MOVE_SPEED = GetParameter("SimpleAttack", "AttackMoveSpeed");

	// ƒXƒLƒ‹‚ÌUŒ‚”{—¦
	const short SKILL_1_ATTACK_RATE_PERCENT = GetParameter("SimpleAttack", "AttackRatePercent");

	// ```````````````````````````ƒXƒLƒ‹2ó‘Ô
#pragma endregion

#pragma region ƒXƒLƒ‹2‚Ì’è”’è‹`

	
#pragma endregion 

#pragma region ƒXƒLƒ‹3‚Ì’è”’è‹`


#pragma endregion 
};