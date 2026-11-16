#pragma once

#include "../BossBase.h"


class BananaScratchCollOperator;
class BananaFireCollOperator;
class BananaBossBanamerang;
class BananaBossDrillManager;

/// <summary>‚Ğ‚Á‚©‚«EƒuƒŒƒXE“Š±UŒ‚‚ÆUŒ‚”ÍˆÍ‚ÌóM“¯Šú‚ğ‘g‚İ—§‚Ä‚éB</summary>
class BananaBoss : public BossBase
{
public:
	BananaBoss();
	~BananaBoss()override = default;

	/// <summary>‹¤’Ê‚ÌóMˆ—‚É‘±‚«A‚±‚Ìƒ{ƒXŒÅ—L‚ÌUŒ‚•\¦‚ğ“¯Šú‚·‚éB</summary>
	virtual void ReceptionUpdate(void)override;

private:

	void BossUpdate(void) override;

	// ƒ_ƒ[ƒWƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
	void PlayDamage(void) override;
	void BossInit(void)override;

#pragma region ’è”’è‹`

	// UŒ‚``````````````````````````````````````````````

	// ‚Ğ‚Á‚©‚«‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int SCRATCH_COOLTIME = GetParameterToInt("Attack", "ScratchCoolTime");
	// ƒuƒŒƒX‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int BREATH_COOLTIME = GetParameterToInt("Attack", "BreathCoolTime");
	// ƒoƒiƒƒ‰ƒ“‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int BANAMERANG_COOLTIME = GetParameterToInt("Attack", "BanamerangCoolTime");
	// ƒoƒiƒhƒŠƒ‹‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int BANADRILL_COOLTIME = GetParameterToInt("Attack", "BanadrillCoolTime");

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

		SCRATCH,
		SCRATCH_START,
		SCRATCH_LOOP,
		SCRATCH_END,

		BREATH,
		BREATH_READY,
		BREATH_READY_LOOP,
		BREATH_ATTACK,
		BREATH_ATTACK_LOOP,

		JUMP,
		JUMP_START,
		JUMP_LOOP,

		DAMAGE,
		DEATH,
		DEATH_FALL,

		MAX
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv‚²‚Æ‚ÌÄ¶‘¬“x
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::MAX] =
	{
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE

		GetParameter("Anime", "ScratchAnimeSpeed"),	// SCRATCH
		GetParameter("Anime", "ScratchAnimeSpeed"),	// SCRATCH
		GetParameter("Anime", "ScratchAnimeSpeed"),	// SCRATCH
		GetParameter("Anime", "ScratchAnimeSpeed"),	// SCRATCH

		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE
		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE
		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE
		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE
		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE

		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE

		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE


		//GetParameter("Anime", "ScratchStartAnimeSpeed"),	// SCRATCH_START
		//GetParameter("Anime", "ScratchLoopAnimeSpeed"),	// SCRATCH_LOOP
		//GetParameter("Anime", "ScratchEndAnimeSpeed"),	// SCRATCH_END

		//GetParameter("Anime", "BreathAnimeSpeed"),	// BREATH
		//GetParameter("Anime", "BreathReadyAnimeSpeed"),	// BREATH_READY
		//GetParameter("Anime", "BreathReadyLoopAnimeSpeed"),	// BREATH_READY_LOOP
		//GetParameter("Anime", "BreathAttackAnimeSpeed"),	// BREATH_ATTACK
		//GetParameter("Anime", "BreathAttackLoopAnimeSpeed"),	// BREATH_ATTACK_LOOP

		//GetParameter("Anime", "JumpAnimeSpeed"),	// JUMP
		//GetParameter("Anime", "JumpStartAnimeSpeed"),	// JUMP_START
		//GetParameter("Anime", "JumpLoopAnimeSpeed"),	// JUMP_LOOP

		//GetParameter("Anime", "DamageAnimeSpeed"),		// DAMAGE
		//GetParameter("Anime", "DeathAnimeSpeed"),		// DEATH
		//GetParameter("Anime", "DeathFallAnimeSpeed"),		// DEATH_FALL
	};

	// ``````````````````````````ƒAƒjƒ[ƒVƒ‡ƒ“

#pragma endregion ’è”’è‹`

	SHADER_VERTEX_TYPE GetVertexType(void) const override { return SHADER_VERTEX_TYPE::NMapFrame8; }
	void DefaultCreateShader(void)override {}

	// ƒƒCƒ“ˆ—``````````````
	void BossLoad(void)override;
	// ``````````````ƒƒCƒ“ˆ—

	// UŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	int attackCooldownFrames;

	// ‚Ğ‚Á‚©‚«UŒ‚‚ÌƒtƒŒ[ƒ€À•W
	Vector3 scratchStartPosition;
	Vector3 scratchEndPosition;

	// Ä‹A•Ï”
	BananaScratchCollOperator* scratchRef;
	BananaFireCollOperator* fireRef;
	BananaBossBanamerang* banamerangRef;
	BananaBossDrillManager* banadrillRef;
};