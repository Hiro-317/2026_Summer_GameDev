#include "GrapeBossWeaponBase.h"

void GrapeBossWeaponBase::Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats)
{
	// ƒXƒLƒ‹‚Ì•t—^
	CreateAttackSkill(operatorSenderId, 50, &stats);
}

void GrapeBossWeaponBase::SubDraw(void)
{
	trans.Draw();
}
