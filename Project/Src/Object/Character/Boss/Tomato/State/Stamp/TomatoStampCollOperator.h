#pragma once

#include "../../../../../ActorBase.h"

class TomatoStampCollOperator : public ActorBase
{
public:

	/// <summary>
	/// ƒRƒ“ƒXƒgƒ‰ƒNƒg
	/// </summary>
	/// <param name="ATTACK_RADIUS">UŒ‚”ÍˆÍ</param>
	/// <param name="ATTACK_DURATION">UŒ‚”»’è‚Ì‘±ŠÔ</param>
	TomatoStampCollOperator(
		float ATTACK_RADIUS,
		int ATTACK_DURATION,
		const bool& isGournd,
		const Vector3& playerPos,
		const CharacterStats stats
	);
	~TomatoStampCollOperator()override = default;

	void Load(void)override;

	void Draw(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	// UŒ‚‚Ì’†SÀ•W‚Ìæ“¾
	Vector3 GetAttackPos(void) { return trans.pos; }

	// UŒ‚”ÍˆÍ‚Ì•`‰æİ’è
	void SetDrawArea(bool flg) { isDrawArea = flg; }

	// UŒ‚I—¹
	bool End(void) { return end; }

private:

#pragma region ’è”

	// UŒ‚”ÍˆÍ‚Ì”¼Œa
	const float ATTACK_RADIUS;

	// UŒ‚‚Ì”­¶‘±ŠÔ
	const int ATTACK_DURATION;

#pragma endregion

#pragma region ó‚¯æ‚éQÆ•Ï”

	const bool& ground;

	const Vector3& playerPos;

	const CharacterStats stats;

#pragma endregion

#pragma region •Ï”

	// UŒ‚I—¹
	bool end;

	// UŒ‚ƒJƒEƒ“ƒg
	int attackCnt;

	// UŒ‚”ÍˆÍ•`‰æƒtƒ‰ƒO
	bool isDrawArea;

#pragma endregion

	// XVˆ—
	void SubUpdate(void)override;

};