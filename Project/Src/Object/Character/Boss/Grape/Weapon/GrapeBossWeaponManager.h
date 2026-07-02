#pragma once
#include "GrapeBossWeaponBase.h"

static constexpr int ArraySum(const int* const array, unsigned char arrayNum) {
	int ret = 0;
	for (int i = 0; i < arrayNum; i++) { ret += array[i]; }
	return ret;
}

struct BombType {

	WeaponType type = WeaponType::None;
	GrapeBossWeaponBase* weaponIns = nullptr;
};

class GrapeBossWeaponManager{
public:

	GrapeBossWeaponManager();
	~GrapeBossWeaponManager();

	void Init(const MSG_SENDER_ID id, const CharacterStats& stats);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// 複製用ハンドル
	int bombModel;

	// 武器の配列
	BombType weapons[ArraySum(WeponDuplicateNum, (int)WeaponType::Max)];
};
