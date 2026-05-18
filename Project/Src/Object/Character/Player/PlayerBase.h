#pragma once

#include "../CharacterBase.h"

class PlayerBase : public CharacterBase
{
public:
	PlayerBase(
		short HP_MAX,
		short ATTACK_POWER,
		short DEFENSE_POWER,
		short SPEED_POWER,

		std::string modelPath,
		Vector3 SCALE, 
		Vector3 MODEL_CENTER_DIFF, 
		Vector3 MODEL_LOCAL_ROT
	);

	PlayerBase(
		short HP_MAX,
		short ATTACK_POWER,
		short DEFENSE_POWER,
		short SPEED_POWER,
		const std::string& parameterPath,

		std::string modelPath,
		Vector3 SCALE,
		Vector3 MODEL_CENTER_DIFF,
		Vector3 MODEL_LOCAL_ROT
	);

	~PlayerBase()override = default;

	void CharacterLoad(void)override;
	void CharacterUiDraw(void)override;

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
	virtual void PlayerUiDraw(void) = 0;

protected:

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

private:



	// ƒƒCƒ“ˆ—``````````````
	void CharactorInit(void)override;
	void CharactorUpdate(void)override;
	void CharactorDraw(void)override;
	void CharactorAlphaDraw(void)override;
	void CharactorRelease(void)override;
	// ``````````````ƒƒCƒ“ˆ—



};
