#pragma once
#include "GrapeBossWeaponBase.h"
#include "../../../../ActorBase.h"


static constexpr int ArraySum(const int* const array, unsigned char arrayNum) {
	int ret = 0;
	for (int i = 0; i < arrayNum; i++) { ret += array[i]; }
	return ret;
}

struct BombType {

	WeaponType type = WeaponType::None;
	GrapeBossWeaponBase* weaponIns = nullptr;
	bool live = false;
};

class GrapeBossWeaponManager : public ActorBase{
public:

	GrapeBossWeaponManager(const MSG_SENDER_ID& operatorSenderId, const CharacterStats& stats);
	~GrapeBossWeaponManager();

	// ほしいボムタイプの配列で返す
	std::vector<BombType> GetWeapons(WeaponType type) {

		// 返り値の設定
		std::vector<BombType> res;

		// 配列分出す
		for (auto weapon : weapons) {
			// 配列内のタイプとほしいものが同じなら
			if (weapon.type == type) {
				// 格納
				res.emplace_back(weapon);
			}
		}
		return res;
	}

	void Load(void) override;

protected:

	void SubUpdate(void) override;
	void SubDraw(void) override;
	void SubRelease(void) override;

private:

#pragma region 参照値

	const MSG_SENDER_ID& operatorSenderId;
	const CharacterStats& stats;

#pragma endregion

	// 複製用ハンドル
	int bombModel;

	// 武器の配列
	BombType weapons[ArraySum(WeponDuplicateNum, (int)WeaponType::Max)];
};
