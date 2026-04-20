#pragma once

#include "../ActorBase.h"

#include "../Common/AnimationController/AnimationController.h"

class CharaSelectObj : public ActorBase
{
public:

	CharaSelectObj();
	~CharaSelectObj()override = default;

	virtual void Load(void)override;

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
	const Vector3 MODEL_LOCAL_ROT = Vector3();

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

	// `````````````````````````````````
	
#pragma region ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[
	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚Ìì¬
	void CreateAnimationController(void) { if (anime == nullptr) anime = new AnimationController(trans.model); }

	/// <summary>
	/// •Êƒtƒ@ƒCƒ‹‚É•Û‘¶‚³‚ê‚Ä‚¢‚éFBXƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	/// </summary>
	/// <param name="index">QÆ”Ô†</param>
	/// <param name="speed">Ä¶‘¬“x</param>
	/// <param name="filePath">ƒpƒX</param>
	void AddAnimation(int index, float speed, const char* filePath);

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

private:

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚ÌƒCƒ“ƒXƒ^ƒ“ƒX
	AnimationController* anime;

	// ”h¶æ’Ç‰ÁXV
	void SubUpdate(void) override;
};