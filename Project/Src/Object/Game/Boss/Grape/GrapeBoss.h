#pragma once

#include "../BossBase.h"

class HitUI;

/// <summary>”š’eƒv[ƒ‹‚ğŠeUŒ‚ƒXƒe[ƒg‚Ö“n‚µAUŒ‚”ÍˆÍ‚Æ”š’e‚ÌóM“¯Šú‚ğs‚¤B</summary>
class GrapeBoss : public BossBase
{
public:
	GrapeBoss();
	~GrapeBoss()override = default;

	/// <summary>‹¤’Ê‚ÌóMˆ—‚É‘±‚«A‚±‚Ìƒ{ƒXŒÅ—L‚ÌUŒ‚•\¦‚ğ“¯Šú‚·‚éB</summary>
	virtual void ReceptionUpdate(void)override;

private:

	// ƒ_ƒ[ƒWƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
	void PlayDamage(void) override;

#pragma region ’è”’è‹`

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
		GetParameter("Anime", "DamageAnimeSpeed"),		// RUN
		GetParameter("Anime", "DownAnimeSpeed"),		// RUN
		GetParameter("Anime", "DeathAnimeSpeed"),		// RUN

		GetParameter("Anime", "KickAnimeSpeed"),	// KICK
		GetParameter("Anime", "OverhandAnimeSpeed"),	// OVERHAND
		GetParameter("Anime", "DownThrowAnimeSpeed"),	// DOWNTHROW
		GetParameter("Anime", "JumpStartAnimeSpeed"),	// JUMP_START
		GetParameter("Anime", "JumpingAnimeSpeed"),	// JUMPING
		GetParameter("Anime", "StampAnimeSpeed"),	// STAMP
	};

	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::MAX] =
	{
		true,
		true,
		true,
		false,
		false,
		false,

		false,
		false,
		false,
		false,
		false,
		false,
	};
	// ``````````````````````````ƒAƒjƒ[ƒVƒ‡ƒ“

#pragma endregion ’è”’è‹`

	// ƒƒCƒ“ˆ—``````````````
	void BossLoad(void)override;
	void BossUpdate(void)override;
	// ``````````````ƒƒCƒ“ˆ—

	// Œü‚¢‚Ä‚¢‚éŒü‚«
	Vector3 moveDir;

	// UŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	float attackCooldownFrames;

	// ƒQ[ƒ€ƒNƒŠƒA‚ÌƒJƒEƒ“ƒgƒ_ƒEƒ“
	float gameOverCnt;


};