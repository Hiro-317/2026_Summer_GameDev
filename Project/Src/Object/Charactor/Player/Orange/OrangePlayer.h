#pragma once

#include "../../CharactorBase.h"

class OrangePlayer : public CharactorBase
{
public:
	OrangePlayer();
	~OrangePlayer()override = default;

	void Load(void)override;

private:

#pragma region ’è”’è‹`

	// ƒ‚ƒfƒ‹``````````````````````````````
	
	// ƒXƒP[ƒ‹
	const Vector3 MODEL_SCALE = GetParameterToVector3("ModelScale");




	// `````````````````````````````````


	// “–‚½‚è”»’èî•ñ``````````````````````````

	// `````````````````````````````````


	// ó‘Ô```````````````````````````````

	// ó‘Ô‚Ìí—Ş
	enum class STATE
	{
		NONE = -1,

		MOVE,
		ATTACK,

		MAX
	};
	


	// `````````````````````````````````


	// ƒAƒjƒ[ƒVƒ‡ƒ“``````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìí—Ş
	enum class ANIME_TYPE
	{
		NONE = -1,

		IDLE,
		WALK,
		RUN,
		//PUNCH_FIRST,
		//PUNCH_SECOND,
		//PUNCH_THIRD,
		//DAMAGE,
		//DEAD,

		MAX
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv‚²‚Æ‚ÌÄ¶‘¬“x
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::MAX] = 
	{
		GetParameter("IdleAnimeSpeed"),			// IDLE
		GetParameter("WalkAnimeSpeed"),			// WALK
		GetParameter("RunAnimeSpeed"),			// RUN
		//GetParameter("PunchFirstAnimeSpeed"),	// PUNCH_FIRST
		//GetParameter("PunchSecondAnimeSpeed"),	// PUNCH_SECOND
		//GetParameter("PunchThirdAnimeSpeed"),	// PUNCH_THIRD
		//GetParameter("DamageAnimeSpeed"),		// DAMAGE
		//GetParameter("DeadAnimeSpeed"),			// DEAD
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌƒtƒHƒ‹ƒ_‚Ü‚Å‚ÌƒpƒX
	const std::string ANIME_FOLDER_PATH = "Data/Model/Charactor/Orange/Anime/";

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv‚²‚Æ‚ÌƒpƒX
	const std::string ANIME_PATH_TABLE[(int)ANIME_TYPE::MAX] =
	{
		ANIME_FOLDER_PATH + "Idle.mv1",			// IDLE
		ANIME_FOLDER_PATH + "Walk.mv1",			// WALK
		ANIME_FOLDER_PATH + "Run.mv1",			// RUN
		//ANIME_FOLDER_PATH + "PunchFirst.mv1",	// PUNCH_FIRST
		//ANIME_FOLDER_PATH + "PunchSecond.mv1",	// PUNCH_SECOND
		//ANIME_FOLDER_PATH + "PunchThird.mv1",	// PUNCH_THIRD
		//ANIME_FOLDER_PATH + "Damage.mv1",		// DAMAGE
		//ANIME_FOLDER_PATH + "Dead.mv1",			// DEAD
	};

	// `````````````````````````````````

#pragma endregion


#pragma region ó‘Ô

#pragma endregion

	// ƒƒCƒ“ˆ—``````````````
	void CharactorInit(void)override;
	void CharactorUpdate(void)override;
	void CharactorDraw(void)override;
	void CharactorAlphaDraw(void)override;
	void CharactorRelease(void)override;
	// ```````````````````
};