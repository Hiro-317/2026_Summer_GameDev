#pragma once

#include "../CharacterBase.h"

#include "CommonPlayerState/TripleAttack/PlayerTripleAttackStDefine.h"


class PlayerBase : public CharacterBase
{
public:
	PlayerBase(
		short HP_MAX,
		short ATTACK_POWER,
		short DEFENSE_POWER,
		short SPEED_POWER,

		std::string modelPath
	);

	PlayerBase(
		MSG_SENDER_ID operatorSenderId,

		const std::string fileName,
		const std::string hpParameterID,
		const std::string attackPowerParameterID,
		const std::string defensePowerParameterID,
		const std::string moveSpeedParameterID,

		const std::string& parameterPath,

		std::string modelPath
	);

	~PlayerBase()override = default;


	// ó‘Ô‘JˆÚŒã1“xs‚¤‰Šú‰»ˆ—
	std::vector<ColliderBase*> GetCollider(void)const override {
		std::vector<ColliderBase*> ret = {};
		// ©g‚ÌƒRƒ‰ƒCƒ_[‚ğ•Ô‹p—pˆê•Ï”‚ÉŠi”[
		for (ColliderBase*& coll : ActorBase::GetCollider()) { ret.emplace_back(coll); }

		// •ø‚¦‚é‰ºˆÊƒNƒ‰ƒX‚Ì•Ô‹p—pˆê•Ï”‚ÉŠi”[
		for (ActorBase* const& subObj : subObjArray) {
			for (ColliderBase*& coll : subObj->GetCollider()) { ret.emplace_back(coll); }
		}
		// ÅI“I‚È•Ô‹p—pˆê•Ï”‚ğ•Ô‹p
		return ret;
	}

	virtual void PlayerLoad(void) = 0;

	const Vector3& GetInterestPos(void)const { return INTEREST_POS; }

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	virtual void ReceptionUpdate(void)override;
	virtual void SendUpdate(void)override;

private:

	// ƒƒCƒ“ˆ—``````````````
	void CharacterLoad(void)override;
	void CharacterInit(void)override;
	void CharacterUpdate(void)override;
	void CharacterRemoteUpdate(void)override;
	void CharacterDraw(void)override;
	void CharacterAlphaDraw(void)override;
	void CharacterUiDraw(void)override;
	void CharacterRelease(void)override;
	// ``````````````ƒƒCƒ“ˆ—


protected:

	void ChangeState(int state)override;
	void AnimePlay(int type, bool loop = true)override;

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

	// å‚ÉÚ’n”»’è‚Ég—p‚·‚éü•ªƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹n“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 LINE_COLLIDER_START_POS = Vector3();
	// å‚ÉÚ’n”»’è‚Ég—p‚·‚éü•ªƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹I“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 LINE_COLLIDER_END_POS = -Vector3::Yonly(MODEL_SIZE.y * 0.5f);
	// å‚ÉÚ’n”»’è‚Ég—p‚·‚éü•ªƒRƒ‰ƒCƒ_[‚Ìâ‘Î‚É“–‚½‚ç‚È‚¢‚¨‚¨‚æ‚»‚Ì‹——£
	const float LINE_COLLIDER_ENOUGH_DISTANCE = LINE_COLLIDER_END_POS.Length();

	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ì”¼Œa
	const float CAPSULE_COLLIDER_RADIUS = (MODEL_SIZE.x * 0.5f) * GetParameter("Collider", "ModelToColliderRate");
	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹n“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_START_POS =
		Vector3::Yonly(
			(MODEL_SIZE.y * 0.5f) * GetParameter("Collider", "ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);
	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹I“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_END_POS =
		-Vector3::Yonly(
			(MODEL_SIZE.y * 0.5f) * GetParameter("Collider", "ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
			- GetParameter("Collider", "ClimbOverHeight")
		);
	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìâ‘Î‚É“–‚½‚ç‚È‚¢‚¨‚¨‚æ‚»‚Ì‹——£
	const float CAPSULE_COLLIDER_ENOUGH_DISTANCE =
		(CAPSULE_COLLIDER_START_POS - CAPSULE_COLLIDER_END_POS).Length()
		+ CAPSULE_COLLIDER_RADIUS;


	// ‰Ÿ‚µo‚µ‚ğs‚¤Û‚Ìd‚³
	const unsigned char COLLISION_PUSH_WEIGHT = (unsigned char)GetParameterToInt("Collider", "CollisionPushWeight");
	// ````````````````````````````````````````````````


	// •Ï”‰Šú‰»Œn``````````````````````````

	// ‰ŠúÀ•W
	const Vector3 INIT_POS = GetParameterToVector3("Init", "Pos");

	// ’‹“_‚Ì‘Š‘ÎÀ•W
	const Vector3 INTEREST_POS = GetParameterToVector3("Init", "InterestPos");

	// ``````````````````````````•Ï”‰Šú‰»Œn

	// ˆÚ“®ó‘Ô`````````````````````````````

	// ‰ÁZˆÚ“®—Ê
	const float MOVE_SPEED = GetParameter("Move","Speed");

	// Å‘åˆÚ“®—Ê
	const float MOVE_SPEED_MAX = GetParameter("Move","SpeedMax");

	// ƒ_ƒbƒVƒ…‚ÌˆÚ“®—Ê”{—¦
	const float DASH_SPEED_RATE = GetParameter("Move", "DashSpeedRate");

	// ƒ_ƒbƒVƒ…‚ÌƒXƒ^ƒ~ƒi‚ÌÅ‘å—Êi1ƒtƒŒ[ƒ€‚¸‚ÂƒfƒNƒŠƒƒ“ƒgj
	const short DASH_STAMINA_MAX = (short)GetParameterToInt("Move", "DashStaminaMax");


	// `````````````````````````````ˆÚ“®ó‘Ô


	// ƒ_ƒ[ƒWó‘Ô`````````````````````````
	
	// ‰ñ”ğ‚Ì–³“G”»’è‚ğ”­¶‚³‚¹‚éŠJnŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const unsigned char DAMAGE_INVI_TIME = (unsigned char)GetParameter("Damage", "DamageInviTime");
	 
	// `````````````````````````ƒ_ƒ[ƒWó‘Ô

	// `````````````````````````````ˆÚ“®ó‘Ô
	 
	// ó‘Ô‚Ìí—Ş•m€
	enum class STATE
	{
		NONE = -1,

		MOVE,			// ˆÚ“®ó‘Ô

		SKILL_1,		// ƒXƒLƒ‹1iŠî–{“I‚Éu’ÊíUŒ‚vj
		SKILL_2,		// ƒXƒLƒ‹2iƒLƒƒƒ‰‚²‚Æ‚Ìu“Áê‹Zvj
		SKILL_3,		// ƒXƒLƒ‹3iƒLƒƒƒ‰‚²‚Æ‚Ìu“Áê‹Zvj
		SPECIAL_SKILL,	// ƒXƒyƒVƒƒƒ‹ƒXƒLƒ‹iƒLƒƒƒ‰‚²‚Æ‚Ìu•KE‹Zvj

		DAMAGE,			// ƒ_ƒ[ƒW
		DEATH,			// “|‚³‚ê‚é

		MAX
	};

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

	// ƒXƒLƒ‹UI‚ÌÀ•W@````````````````````````````````
	const Vector2I SKILL1_UI_DRAW_POS = Vector2I(Application::SCREEN_SIZE_X_HALF + 600, Application::SCREEN_SIZE_Y_HALF + 300);
	const Vector2I SKILL2_UI_DRAW_POS = Vector2I(SKILL1_UI_DRAW_POS.x - 120, SKILL1_UI_DRAW_POS.y - 100);
	const Vector2I SKILL3_UI_DRAW_POS = Vector2I(SKILL1_UI_DRAW_POS.x - 240, SKILL1_UI_DRAW_POS.y);
	// ````````````````````````````````@ƒXƒLƒ‹UI‚ÌÀ•W@
#pragma endregion ’è”’è‹`

	// ’‹“_À•W
	Vector3 interestPos;

};
