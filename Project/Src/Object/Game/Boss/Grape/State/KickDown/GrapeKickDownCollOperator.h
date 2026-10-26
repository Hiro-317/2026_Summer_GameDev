#pragma once

#include "../../../../../Common/ActorBase/ActorBase.h"

struct CharacterStats;

class GrapeKickDownCollOperator : public ActorBase
{
public:
	/// <summary>
	/// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	/// </summary>
	/// <param name="stats">ƒXƒe[ƒ^ƒX</param>
	GrapeKickDownCollOperator(
		const MSG_SENDER_ID& controlSenderId,
		const CharacterStats& stats
	);
	~GrapeKickDownCollOperator()override = default;

	void Load(void)override;

	void CollSet(bool flg) { SetJudgeFlg(flg); }

	void SetPos(const Vector3& pos) { trans.pos = pos; }
	void SetScale(const float& scale) { collFront.scale = Vector3(SCALE.x * scale, 1.0f, SCALE.z * scale); }

	// UŒ‚”ÍˆÍ‚Ì•`‰æİ’è
	void SetDrawArea(bool flg) { isDrawArea = flg; }

private:

	// UŒ‚”»’è—p”¼Œa
	const float ATTACK_RADIUS = 300.0f;

	// UŒ‚”ÍˆÍ•`‰æ‚Ì‚‚³
	const float HEIGHT = 37.0f;

	// UŒ‚”ÍˆÍ•`‰æ‚ÌŠg‘å—¦
	const Vector3 SCALE = Vector3(ATTACK_RADIUS / 100.0f, 1.0f, ATTACK_RADIUS / 100.0f);

#pragma region ó‚¯æ‚éQÆ•Ï”

	const Vector3& bossPos;

	const MSG_SENDER_ID& controlSenderId;

	const CharacterStats& stats;

#pragma endregion

	void SubUpdate(void) override;
	void SubDraw(void) override;
	void SubRelease(void) override;

	Transform collBack;
	Transform collFront;

	// UŒ‚”ÍˆÍ•`‰æƒtƒ‰ƒO
	bool isDrawArea;
};