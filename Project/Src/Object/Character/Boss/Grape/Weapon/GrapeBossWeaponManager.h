#pragma once

static constexpr int ArraySum(const int* const array, unsigned char arrayNum) {
	int ret = 0;
	for (int i = 0; i < arrayNum; i++) { ret += array[i]; }
	return ret;
}

#include "GrapeBossWeaponBase.h"

class GrapeBossWeaponManager{
public:

#pragma region ’è”’è‹`

	// Ží—Þ‚Ì—ñ‹“Œ^’è‹`
	enum class WeaponType {
		None=-1,

		// ’¼ü“Š±
		Straight,

		// ”š’e“Š‰ºiR‚èUŒ‚‚Æ“¯Žž‚Ég‚Ì‰ñ‚è‚É—Ž‚Æ‚·”š’ej
		KickBomb,

		// ”š’e“Š‰ºiƒXƒ^ƒ“ƒvUŒ‚‚Æ“¯Žž‚Ég‚Ì‰ñ‚è‚É—Ž‚Æ‚·”š’ej
		StampBomb,

		// ”š’e“Š‰ºiƒvƒŒƒCƒ„[‚ÉŒü‚©‚Á‚Ä~‚ç‚¹‚é”š’ej
		SingleBomb,

		// ”š’e“Š‰ºi1‘Ì‚ÌƒvƒŒƒCƒ„[‚Éˆê’èŽžŠÔ~‚ç‚¹‘±‚¯‚é”š’ej
		StalkerBomb,

		// ”š’e“Š‰ºiƒ‰ƒ“ƒ_ƒ€‚É’nã‚É‘å—Ê“Š‰º‚·‚é”š’ej
		RandomBomb,

		Max
	};

	static constexpr int WeponDuplicateNum[(int)WeaponType::Max] = {
		// ’¼ü“Š±
		3,

		// ”š’e“Š‰ºiR‚èUŒ‚‚Æ“¯Žž‚Ég‚Ì‰ñ‚è‚É—Ž‚Æ‚·”š’ej
		7,

		// ”š’e“Š‰ºiƒXƒ^ƒ“ƒvUŒ‚‚Æ“¯Žž‚Ég‚Ì‰ñ‚è‚É—Ž‚Æ‚·”š’ej
		10,

		// ”š’e“Š‰ºiƒvƒŒƒCƒ„[‚ÉŒü‚©‚Á‚Ä~‚ç‚¹‚é”š’ej
		1,

		// ”š’e“Š‰ºi1‘Ì‚ÌƒvƒŒƒCƒ„[‚Éˆê’èŽžŠÔ~‚ç‚¹‘±‚¯‚é”š’ej
		5,

		// ”š’e“Š‰ºiƒ‰ƒ“ƒ_ƒ€‚É’nã‚É‘å—Ê“Š‰º‚·‚é”š’ej
		20
	};

#pragma endregion

	GrapeBossWeaponManager();
	~GrapeBossWeaponManager();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	int bombModel;

	struct BombType {

		WeaponType type = WeaponType::None;
		GrapeBossWeaponBase* weaponIns = nullptr;
	};

	BombType weapons[ArraySum(WeponDuplicateNum, (int)WeaponType::Max)];
};
