#pragma once

#include "../Charactor/CharactorBase.h"

class CharaSelectObj : public CharactorBase
{
public:

	CharaSelectObj();
	~CharaSelectObj()override = default;

	virtual void Load(void)override {};

protected:

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

	// •Ï”‰Šú‰»Œn``````````````````````````

	// ‰ŠúÀ•W
	const Vector3 INIT_POS = GetParameterToVector3("InitPos");

	// ````````````````````````````````

	// ó‘Ô‚Ìí—Ş
	enum class STATE
	{
		NONE = -1,

		DANCE,			// ‘Ò‹@ó‘Ô
		MOVE,			// ˆÚ“®ó‘Ô‚ğ’Ç‰Á

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

	// ƒAƒjƒ[ƒVƒ‡ƒ“``````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìí—Ş
	enum class ANIME_TYPE
	{
		NONE = -1,

		DANCE,
		RUN,

		MAX
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv‚²‚Æ‚ÌÄ¶‘¬“x
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::MAX] =
	{
		GetParameter("IdleAnimeSpeed"),		// DANCE
		GetParameter("WalkAnimeSpeed"),		// RUN
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌƒtƒHƒ‹ƒ_‚Ü‚Å‚ÌƒpƒX
	const std::string ANIME_FOLDER_PATH = "Data/Model/Charactor/CommonAnime/";

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv‚²‚Æ‚ÌƒpƒX
	const std::string ANIME_PATH_TABLE[(int)ANIME_TYPE::MAX] =
	{
		ANIME_FOLDER_PATH + "Dance.mv1",		// DANCE
		ANIME_FOLDER_PATH + "Run.mv1",			// RUN
	};

	// `````````````````````````````````
};