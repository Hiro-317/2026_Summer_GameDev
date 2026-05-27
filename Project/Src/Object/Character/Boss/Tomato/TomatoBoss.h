#pragma once
#include "../../CharacterBase.h"


class TomatoBoss : public CharacterBase
{
public:
	TomatoBoss(const Vector3& playerPos);
	~TomatoBoss()override = default;

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

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)override;

private:

#pragma region ’è”’è‹`

	// ƒ‚ƒfƒ‹``````````````````````````````

	// ƒXƒP[ƒ‹
	const Vector3 MODEL_SCALE = GetParameterToVector3("Model","Scale");

	// ƒTƒCƒY
	const Vector3 MODEL_SIZE = GetParameterToVector3("Model","Size") * MODEL_SCALE;

	// ’†S“_‚ÌƒYƒŒ
	const Vector3 MODEL_CENTER_DIFF = GetParameterToVector3("Model","CenterDiff") * MODEL_SCALE;

	// Šp“x‚ÌƒYƒŒ
	const Vector3 MODEL_LOCAL_ROT = GetParameterToVector3("Model","LocalRot") * (DX_PI_F / 180.0f);

	// `````````````````````````````````


	// “–‚½‚è”»’èî•ñ`````````````````````````````````````````

	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ì”¼Œa
	const float CAPSULE_COLLIDER_RADIUS = (MODEL_SIZE.y * 0.5f) * GetParameter("Collider","ModelToColliderRate");

	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹Xn“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_START_POS_X =
		Vector3::Xonly(
			(MODEL_SIZE.x * 0.5f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);
	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹XI“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_END_POS_X =
		-Vector3::Xonly(
			(MODEL_SIZE.x * 0.5f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);

	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹XZn“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_START_POS_XZ =
		Vector3::XZonly(
			(MODEL_SIZE.x * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS,
			(MODEL_SIZE.z * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);
	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹XZI“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_END_POS_XZ =
		-Vector3::XZonly(
			(MODEL_SIZE.x * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS,
			(MODEL_SIZE.z * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);

	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹Zn“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_START_POS_Z =
		Vector3::Zonly(
			(MODEL_SIZE.z * 0.5f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);
	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹ZI“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_END_POS_Z =
		-Vector3::Zonly(
			(MODEL_SIZE.z * 0.5f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);

	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹ZXn“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_START_POS_ZX =
		Vector3::XZonly(
			-((MODEL_SIZE.x * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS),
			(MODEL_SIZE.z * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);
	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹ZXI“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_END_POS_ZX =
		-Vector3::XZonly(
			-((MODEL_SIZE.x * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS),
			(MODEL_SIZE.z * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);

	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìâ‘Î‚É“–‚½‚ç‚È‚¢‚¨‚¨‚æ‚»‚Ì‹——£
	const float CAPSULE_COLLIDER_ENOUGH_DISTANCE =
		(CAPSULE_COLLIDER_START_POS_X - CAPSULE_COLLIDER_END_POS_X).Length()
		+ CAPSULE_COLLIDER_RADIUS;

	// ƒvƒŒƒCƒ„[‚É‚¾‚¯s‚¤(XZ•½–Ê‚Ì‚İ‚ğQÆ‚·‚é)‰Ÿ‚µo‚µ‚Ì‹——£
	const float TO_PLAYER_DISTANCE = 250.0f;

	// ‰Ÿ‚µo‚µ‚ğs‚¤Û‚Ìd‚³
	const unsigned char COLLISION_PUSH_WEIGHT = (unsigned char)GetParameterToInt("Collider", "CollisionPushWeight");
	// ````````````````````````````````````````````````


	// •Ï”‰Šú‰»Œn``````````````````````````

	// ‰ŠúÀ•W
	const Vector3 INIT_POS = GetParameterToVector3("Init", "Pos");

	// ``````````````````````````•Ï”‰Šú‰»Œn

	// ó‘Ô‚Ìí—Ş
	enum class STATE
	{
		NONE = -1,
		
		IDLE,
		HEADBUTT,
		MOVE,
		TACKLE,
		STAMP,

		MAX
	};

	// ˆÚ“®ó‘Ô`````````````````````````````

	// ˆÚ“®—Ê
	const float MOVE_SPEED = GetParameter("Move", "Speed");

	// ‰ñ“]—Ê
	const float ROTATION_POW = GetParameter("Move", "RotationPower");

	// `````````````````````````````ˆÚ“®ó‘Ô


	// UŒ‚```````````````````````````````

	// ƒXƒ^ƒ“ƒv”ÍˆÍ
	const float STAMP_RADIUS = GetParameter("Attack", "StampRad");

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
	void CharacterLoad(void)override;
	void CharactorInit(void)override;
	void CharactorUpdate(void)override;
	void CharactorDraw(void)override;
	void CharactorAlphaDraw(void)override;
	void CharacterUiDraw(void)override;
	void CharactorRelease(void)override;
	// ``````````````ƒƒCƒ“ˆ—

	// ƒvƒŒƒCƒ„[‚ÌÀ•W‚ÌQÆ(“Ç‚İæ‚èê—p)
	const Vector3& playerPos;

	// ƒgƒ}ƒg‚ÌŒü‚¢‚Ä‚¢‚éŒü‚«
	Vector3 moveDir;

	// Šâ‚É“–‚½‚Á‚½‚©
	bool rockHit;

	// UŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	int coolTime;
};