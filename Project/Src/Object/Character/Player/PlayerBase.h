#pragma once

#include "../../../Manager/Net/NetWorkManager.h"

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

	/// <summary>
	/// ƒvƒŒƒCƒ„[ƒx[ƒX‚ÌƒRƒ“ƒXƒgƒ‰ƒNƒ^
	/// </summary>
	/// <param name="operatorSenderId">’N‚ª‘€ìŒ ‚ğ‚Â‚©</param>
	/// <param name="fileName">ƒpƒ‰ƒ[ƒ^[‚ÌƒpƒX</param>
	/// <param name="hpParameterID">HPƒpƒ‰ƒ[ƒ^[‚Ì–¼‘O</param>
	/// <param name="attackPowerParameterID">UŒ‚—Íƒpƒ‰ƒ[ƒ^[‚Ì–¼‘O</param>
	/// <param name="defensePowerParameterID">–hŒä—Íƒpƒ‰ƒ[ƒ^[‚Ì–¼‘O</param>
	/// <param name="moveSpeedParameterID">ˆÚ“®‘¬“xƒpƒ‰ƒ[ƒ^[‚Ì–¼‘O</param>
	/// <param name="parameterPath">ŠO•”‚©‚çƒpƒ‰ƒ[ƒ^[‚ğ“Ç‚İ‚Ş‚½‚ß‚Ìƒtƒ@ƒCƒ‹ƒpƒX</param>
	/// <param name="modelPath">ƒvƒŒƒCƒ„[ƒ‚ƒfƒ‹‚Ìƒtƒ@ƒCƒ‹ƒpƒX</param>
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

	// ƒ{ƒX‚ÌÀ•Wæ“¾
	void SetBossPos(const Vector3* bossPos) { this->bossPos = bossPos; }

	// ƒ[ƒh
	virtual void PlayerLoad(void) = 0;

	const Vector3& GetInterestPos(void) const { return INTEREST_POS; }
	const MSG_SENDER_ID& GetOperatorSenderId(void) const { return operatorSenderId; }

	// ©gˆÈŠO‚ÌƒvƒŒƒCƒ„[À•W‚ğæ“¾
	void SetOtherPlayerTrans(const Transform* pos) { otherPlayerTrans.emplace_back(pos); }

	// “–‚½‚è”»’èˆ—
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

	// ƒXƒe[ƒg‘JˆÚŠÖ”
	void ChangeState(int state)override;

	// ƒ‚ƒfƒ‹ƒAƒjƒ[ƒVƒ‡ƒ“ƒvƒŒƒCŠÖ”
	void AnimePlay(int type, bool loop = true)override;

	void PlayerDeathSetting(void) { SetJudge(false); SetIsDraw(false); }

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
	const unsigned char DODGE_INVI_TIME = (unsigned char)GetParameter("Damage", "DamageInviTime");
	 
	// `````````````````````````ƒ_ƒ[ƒWó‘Ô

	// `````````````````````````````ˆÚ“®ó‘Ô
	 
	// ó‘Ô‚Ìí—Ş
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
		OTHER_WATCH,

		MAX
	};

	// •ø‚¦‚é‰ºˆÊƒNƒ‰ƒX‚ğŠi”[‚·‚é”z—ñ
	std::vector<ActorBase*> subObjArray;
	// •ø‚¦‚é‰ºˆÊƒNƒ‰ƒX‚ğŠi”[‚·‚é”z—ñ‚Ì’†‚©‚ç“Á’è‚ÌƒIƒuƒWƒFƒNƒg‚ğ’T‚·
	template<typename SubClass = ActorBase>
	SubClass* SubObjSerch(unsigned char index = 0) {
		for (ActorBase* obj : subObjArray) {
			if (dynamic_cast<SubClass*>(obj)) {
				if (index > 0) { index--; continue; }
				return dynamic_cast<SubClass*>(obj);
			}
		}
		return nullptr;
	}

	// UI@````````````````````````````````
	const Vector2I SKILL1_UI_DRAW_POS = Vector2I(Application::SCREEN_SIZE_X_HALF + 600, Application::SCREEN_SIZE_Y_HALF + 300);
	const Vector2I SKILL2_UI_DRAW_POS = Vector2I(SKILL1_UI_DRAW_POS.x - 120, SKILL1_UI_DRAW_POS.y - 100);
	const Vector2I SKILL3_UI_DRAW_POS = Vector2I(SKILL1_UI_DRAW_POS.x - 240, SKILL1_UI_DRAW_POS.y);

	// ‰æ‘œ‚Ì–¼‘O
	const std::string HP_FRAME_IMAGE_NAME = "PlayerHpFrame";
	const std::string HP_IMAGE_NAME = "PlayerHp";
	const std::string HP_LOST_IMAGE_NAME = "PlayerHpLost";

	// HPƒo[‚Ì‰æ‘œƒTƒCƒY
	const Vector2I HP_IMAGE_SIZE = Vector2I(354, 50);
	const Vector2I HP_GAUGE_OFFSET = Vector2I(10, 0);
	const Vector2I HP_UI_POS[(int)MSG_SENDER_ID::Max] = {
		Vector2I(0, App::SCREEN_SIZE_Y_HALF),
		Vector2I(0, App::SCREEN_SIZE_Y_HALF + (HP_IMAGE_SIZE.y + 20)),
		Vector2I(0, App::SCREEN_SIZE_Y_HALF + ((HP_IMAGE_SIZE.y + 20) * 2)),
		Vector2I(0, App::SCREEN_SIZE_Y_HALF + ((HP_IMAGE_SIZE.y + 20) * 3)),
	};

	// ````````````````````````````````@UI
#pragma endregion ’è”’è‹`

	// ’‹“_À•W
	Vector3 interestPos;

	// ©gˆÈŠO‚ÌƒvƒŒƒCƒ„[‚ÌÀ•W
	std::vector<const Transform*>otherPlayerTrans;

	// ƒ{ƒX‚ÌÀ•W‚Ìƒ|ƒCƒ“ƒ^
	const Vector3* bossPos;

	// –¡•ûƒvƒŒƒCƒ„[“à‚Ìƒ^[ƒQƒbƒg
	unsigned char targetPlayerIndex;

	// –¡•ûƒvƒŒƒCƒ„[“à‚Ìƒ^[ƒQƒbƒgQÆÀ•W
	const Vector3* targetPlayerPos;

	// –¡•ûƒvƒŒƒCƒ„[‚Ìƒ^[ƒQƒbƒg‚ğŸ‚Ö
	void TargetPlayerNext(void);

};
