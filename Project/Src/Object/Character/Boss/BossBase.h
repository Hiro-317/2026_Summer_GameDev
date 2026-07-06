#pragma once

#include "../../../Manager/Net/NetWorkManager.h"

#include "../CharacterBase.h"


class BossBase : public CharacterBase
{
public:

	BossBase(

		const std::string fileName,
		const std::string hpParameterID,
		const std::string attackPowerParameterID,
		const std::string defensePowerParameterID,
		const std::string moveSpeedParameterID,

		const std::string& parameterPath,

		std::string modelPath,

		const std::vector<const Vector3*> playerPos
	);

	~BossBase()override = default;


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

	const MSG_SENDER_ID& GetOperatorSenderId(void) const { return operatorSenderId; }

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	virtual void ReceptionUpdate(void)override;
	virtual void SendUpdate(void)override;

private:

	// ƒƒCƒ“ˆ—``````````````
	void CharacterLoad(void)override;
	void CharacterInit(void)override;
	void CharacterUpdate(void)override;
	void CharacterDraw(void)override;
	void CharacterAlphaDraw(void)override;
	void CharacterUiDraw(void)override;
	void CharacterRelease(void)override;
	// ``````````````ƒƒCƒ“ˆ—

protected:

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

	// ƒvƒŒƒCƒ„[‚É‚¾‚¯s‚¤(XZ•½–Ê‚Ì‚İ‚ğQÆ‚·‚é)‰Ÿ‚µo‚µ‚Ì‹——£
	const float TO_PLAYER_DISTANCE = 250.0f;

	// ‰Ÿ‚µo‚µ‚ğs‚¤Û‚Ìd‚³
	const unsigned char COLLISION_PUSH_WEIGHT = (unsigned char)GetParameterToInt("Collider", "CollisionPushWeight");
	// ````````````````````````````````````````````````


	// •Ï”‰Šú‰»Œn``````````````````````````

	// ‰ŠúÀ•W
	const Vector3 INIT_POS = GetParameterToVector3("Init", "Pos");

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

	// ƒvƒŒƒCƒ„[‚ÌÀ•W‚ÌQÆ(“Ç‚İæ‚èê—p)
	const std::vector<const Vector3*> playerPos;

	// ƒRƒ‰ƒCƒ_[‚Ì“–‚½‚è”»’è•`‰æ‚Ìƒpƒ‰ƒ[ƒ^[
	ParameterLoad* collParam;

	// ƒvƒŒƒCƒ„[‚Ì—^ƒ_ƒƒJƒEƒ“ƒg
	std::vector<int> damaged;

	// ƒvƒŒƒCƒ„[‚ÌÅ—^ƒ_ƒ/‚»‚Ì”Ô†
	int mostDamaged;
	int targetNum;
};
