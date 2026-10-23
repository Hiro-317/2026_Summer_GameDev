#pragma once

#include "../../../Application/Application.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../Common/CharacterBase/CharacterBase.h"

class HitUI;

class BossBase : public CharacterBase
{
public:

	BossBase(const std::string& parameterPath, std::string modelPath);

	~BossBase()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	virtual void ReceptionUpdate(void)override;
	virtual void SendUpdate(void)override;

	const bool& GetIsDeath(void)const { return false; }

	void SetPlayerPos(MSG_SENDER_ID id, const Vector3* pos) { playerPos[(int)id] = pos; }
	void SetPlayerIsDeath(MSG_SENDER_ID id, const bool* isDeath) { playerIsDeath[(int)id] = isDeath; }

private:

	// ƒƒCƒ“ˆ—``````````````
	void SubInit(void)override;
	void SubUpdate(void)override;
	void SubRelease(void)override;
	// ``````````````ƒƒCƒ“ˆ—

protected:

	virtual void BossLoad(void) = 0;
	virtual void BossInit(void) {};

	// ƒXƒe[ƒg‘JˆÚŠÖ”
	void ChangeState(int nextState)override;
	using CharacterBase::ChangeState;

	// ƒ‚ƒfƒ‹ƒAƒjƒ[ƒVƒ‡ƒ“ƒvƒŒƒCŠÖ”
	void AnimePlay(int type, signed char loop = -1)override;
	using CharacterBase::AnimePlay;

	virtual void PlayDamage(void){}

#pragma region ’è”’è‹`
	// ƒ‚ƒfƒ‹``````````````````````````````

	// ƒXƒP[ƒ‹
	const Vector3 MODEL_SCALE = GetParameterToVector3("Model", "Scale");

	// ƒTƒCƒY
	const Vector3 MODEL_SIZE = GetParameterToVector3("Model", "Size") * MODEL_SCALE;

	// ’†S“_‚ÌƒYƒŒ
	const Vector3 MODEL_CENTER_DIFF = GetParameterToVector3("Model", "CenterDiff") * MODEL_SCALE;

	// Šp“x‚ÌƒYƒŒ
	const Vector3 MODEL_LOCAL_ROT = GetParameterToVector3("Model", "LocalRot") * (DX_PI_F / 180.0f);

	// `````````````````````````````````


	// “–‚½‚è”»’èî•ñ`````````````````````````````````````````

	// ‰Ÿ‚µo‚µ‚ğs‚¤Û‚Ìd‚³
	const unsigned char COLLISION_PUSH_WEIGHT = (unsigned char)GetParameterToInt("Collider", "CollisionPushWeight");

	// ````````````````````````````````````````````````


	// •Ï”‰Šú‰»Œn``````````````````````````

	// ‰ŠúÀ•W
	const Vector3 INIT_POS = GetParameterToVector3("Init", "Pos");
	const Vector3 INIT_ROT = GetParameterToVector3("Init", "Rot") * DX_PI_F / 180.0f;

	// ``````````````````````````•Ï”‰Šú‰»Œn

	// ˆÚ“®ó‘Ô`````````````````````````````

	// ˆÚ“®—Ê
	const float MOVE_SPEED = GetParameter("Move", "Speed");

	// `````````````````````````````ˆÚ“®ó‘Ô

	// `````````````````````````````ˆÚ“®ó‘Ô

	// ó‘Ô‚Ìí—Ş•m€
	enum class STATE
	{
		NONE = -1,

		IDLE,			// ‘Ò‹@ó‘Ô
		MOVE,			// ˆÚ“®ó‘Ô

		ATTACK_A,
		ATTACK_B,
		ATTACK_C,
		ATTACK_D,
		ATTACK_E,
		ATTACK_F,
		ATTACK_G,

		DEATH,

		MAX
	};

	// ƒXƒLƒ‹UI‚ÌÀ•W@````````````````````````````````
	const Vector2I SKILL1_UI_DRAW_POS = Vector2I(Application::SCREEN_SIZE_X_HALF + 600, Application::SCREEN_SIZE_Y_HALF + 300);
	const Vector2I SKILL2_UI_DRAW_POS = Vector2I(SKILL1_UI_DRAW_POS.x - 120, SKILL1_UI_DRAW_POS.y - 100);
	const Vector2I SKILL3_UI_DRAW_POS = Vector2I(SKILL1_UI_DRAW_POS.x - 240, SKILL1_UI_DRAW_POS.y);
	// ````````````````````````````````@ƒXƒLƒ‹UI‚ÌÀ•W@
#pragma endregion ’è”’è‹`

	// ƒvƒŒƒCƒ„[‚ÌÀ•W‚ÌQÆ(“Ç‚İæ‚èê—p)
	const Vector3* playerPos[(int)MSG_SENDER_ID::Max];

	// ƒvƒŒƒCƒ„[‚Ì¶‘¶”»’è‚ÌQÆ(“Ç‚İæ‚èê—p)
	const bool* playerIsDeath[(int)MSG_SENDER_ID::Max];

	// ƒvƒŒƒCƒ„[‚Ì—^ƒ_ƒƒJƒEƒ“ƒg
	int damaged[(int)MSG_SENDER_ID::Max];

	// Œ»İ‚Ìƒ^[ƒQƒbƒgƒvƒŒƒCƒ„[”Ô†
	int targetNum;


	// ƒRƒ‰ƒCƒ_[‚Ì“–‚½‚è”»’è•`‰æ‚Ìƒpƒ‰ƒ[ƒ^[
	ParameterLoad* collParam;

	// ƒ_ƒ[ƒWUIŠÇ—ƒNƒ‰ƒX
	HitUI* hitUiRef;
};
