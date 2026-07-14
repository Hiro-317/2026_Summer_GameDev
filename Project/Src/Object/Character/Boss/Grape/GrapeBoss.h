#pragma once
#include "../BossBase.h"


class GrapeBoss : public BossBase
{
public:
	GrapeBoss(const std::vector<const Vector3*> playerPos, const std::vector<const bool*> playerLive);
	~GrapeBoss()override = default;

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

	virtual void ReceptionUpdate(void)override;

private:

#pragma region ’è”’è‹`

	// “–‚½‚è”»’èî•ñ`````````````````````````````````````````

	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ì”¼Œa
	const float CAPSULE_COLLIDER_RADIUS = (MODEL_SIZE.y * 0.5f) * GetParameter("Collider", "ModelToColliderRate");

	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹Xn“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_START_POS =
		Vector3::Xonly(
			(MODEL_SIZE.x * 0.5f) * GetParameter("Collider", "ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);
	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìƒ[ƒJƒ‹XI“_À•Wiƒ‚ƒfƒ‹‚Ì’†S“_‚©‚ç‚ÌƒIƒtƒZƒbƒgj
	const Vector3 CAPSULE_COLLIDER_END_POS =
		-Vector3::Xonly(
			(MODEL_SIZE.x * 0.5f) * GetParameter("Collider", "ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);

	// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[‚Ìâ‘Î‚É“–‚½‚ç‚È‚¢‚¨‚¨‚æ‚»‚Ì‹——£
	const float CAPSULE_COLLIDER_ENOUGH_DISTANCE =
		(CAPSULE_COLLIDER_START_POS - CAPSULE_COLLIDER_END_POS).Length()
		+ CAPSULE_COLLIDER_RADIUS;

	// ````````````````````````````````````````````````


	// UŒ‚``````````````````````````````````````````````

	// ‚©‚©‚Æ—‚Æ‚µ‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int KICKDOWN_COOLTIME = GetParameterToInt("Attack", "KickDownCoolTime");
	// ’¼ü“Š±‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int STRAIGHT_COOLTIME = GetParameterToInt("Attack", "StraightCoolTime");
	// ƒXƒ^ƒ“ƒvUŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int STAMP_COOLTIME = GetParameterToInt("Attack", "StampCoolTime");
	// ˆê‚Â‚ÌUŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int SINGLE_COOLTIME = GetParameterToInt("Attack", "SingleCoolTime");
	// ’Ç]UŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int STALKER_COOLTIME = GetParameterToInt("Attack", "StalkerCoolTime");
	// ƒ‰ƒ“ƒ_ƒ€UŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int RANDOM_COOLTIME = GetParameterToInt("Attack", "RandomCoolTime");

	// ``````````````````````````````````````````````UŒ‚


	// ˆÚ“®ó‘Ô`````````````````````````````

	// ˆÚ“®—Ê
	const float MOVE_SPEED = GetParameter("Move", "Speed");

	// `````````````````````````````ˆÚ“®ó‘Ô


	// ‰æ‘œ‚Ì–¼‘O
	const std::string HP_FRAME_IMAGE_NAME = "BossHpFrame";
	const std::string HP_IMAGE_NAME = "BossHp";
	const std::string HP_LOST_IMAGE_NAME = "BossHpLost";

	// HPƒo[‚Ì‰æ‘œƒTƒCƒY
	const Vector2I HP_IMAGE_SIZE = Vector2I(629, 50);
	const Vector2I HP_GAUGE_OFFSET = Vector2I(18, 0);
	const Vector2I HP_UI_POS = Vector2I(App::SCREEN_SIZE_X_HALF - 314, 0);

	// ƒQ[ƒ€ƒNƒŠƒA‚Ì•ÏXŠÔ
	static constexpr int GAMECLEAR_CHANGE_TIME = 120;

	// ƒAƒjƒ[ƒVƒ‡ƒ“``````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìí—Ş
	enum class ANIME_TYPE
	{
		NONE = -1,

		IDLE,
		WALK,
		RUN,
		DAMAGE,
		DOWN,
		DEATH,

		KICKDOWN,
		FORWARD_THROW,
		TOSS,
		JUMP_START,
		JUMP_LOOP,
		STAMP, 

		MAX
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv‚²‚Æ‚ÌÄ¶‘¬“x
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::MAX] =
	{
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "WalkAnimeSpeed"),	// WALK
		GetParameter("Anime", "RunAnimeSpeed"),		// RUN
		GetParameter("Anime", "RunAnimeSpeed"),		// RUN
		GetParameter("Anime", "RunAnimeSpeed"),		// RUN
		GetParameter("Anime", "RunAnimeSpeed"),		// RUN

		GetParameter("Anime", "KickAnimeSpeed"),	// KICK
		GetParameter("Anime", "OverhandAnimeSpeed"),	// OVERHAND
		GetParameter("Anime", "DownThrowAnimeSpeed"),	// DOWNTHROW
		GetParameter("Anime", "JumpStartAnimeSpeed"),	// JUMP_START
		GetParameter("Anime", "JumpingAnimeSpeed"),	// JUMPING
		GetParameter("Anime", "StampAnimeSpeed"),	// STAMP
	};

	// ``````````````````````````ƒAƒjƒ[ƒVƒ‡ƒ“

#pragma endregion ’è”’è‹`

	// ƒƒCƒ“ˆ—``````````````
	void PlayerLoad(void)override;
	// ``````````````ƒƒCƒ“ˆ—

	// ƒgƒ}ƒg‚ÌŒü‚¢‚Ä‚¢‚éŒü‚«
	Vector3 moveDir;

	// UŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	int coolTime;

	// ƒQ[ƒ€ƒNƒŠƒA‚ÌƒJƒEƒ“ƒgƒ_ƒEƒ“
	int gameOverCnt;

	
};