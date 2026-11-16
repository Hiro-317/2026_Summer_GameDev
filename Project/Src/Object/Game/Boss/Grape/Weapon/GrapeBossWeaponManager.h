#pragma once
#include "GrapeBossWeaponBase.h"

#include "../../../../../Manager/Net/SenderID_Define.h"

struct CharacterStats;

static constexpr int ArraySum(const int* const array, unsigned char arrayNum) {
	int ret = 0;
	for (int i = 0; i < arrayNum; i++) { ret += array[i]; }
	return ret;
}

struct BombPoolEntry {

	WeaponType type = WeaponType::None;
	GrapeBossWeaponBase* weaponActor = nullptr;
	bool isActive = false;
};

class GrapeBossWeaponManager : public ActorBase{
public:

	/// <summary>種類ごとの爆弾を事前生成し、再利用用の連続した領域に登録する。</summary>
	/// <param name="operatorSenderId">操作権の送信者IDへの参照。マネージャー利用中は有効であること。</param>
	/// <param name="stats">攻撃に使うボス能力値への参照。</param>
	GrapeBossWeaponManager(const MSG_SENDER_ID& operatorSenderId, const CharacterStats& stats);
	~GrapeBossWeaponManager() = default;

	/// <summary>指定種類の先頭要素を返す。該当なしの場合はnullptr。</summary>
	/// <param name="type">取得する爆弾の種類。</param>
	/// <returns>内部配列の借用ポインター。添字は該当種類のWeaponPoolCounts未満に制限する。</returns>
	BombPoolEntry* GetWeaponPool(WeaponType type) {

		// 同じ種類を連続配置しているため、最初の一致がそのプールの先頭になる。
		for (auto& weapon : weapons) {
			if (weapon.type == type) {
				return &weapon;
			}
		}
		// 未登録の種類は取得できない。
		return nullptr;
	}

	/// <summary>種類内の添字を検証し、爆弾プールの要素を取得する。</summary>
	/// <param name="type">取得する爆弾の種類。</param>
	/// <param name="index">種類内の添字。負数や個数以上の値は無効。</param>
	/// <returns>内部要素への借用ポインター。種類・添字が無効ならnullptr。</returns>
	BombPoolEntry* GetWeapon(WeaponType type, int index) {
		const int typeIndex = static_cast<int>(type);
		if (typeIndex < 0 || typeIndex >= static_cast<int>(WeaponType::Max)
			|| index < 0 || index >= WeaponPoolCounts[typeIndex]) { return nullptr; }
		BombPoolEntry* pool = GetWeaponPool(type);
		return pool != nullptr ? &pool[index] : nullptr;
	}

	void Load(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// 例外処理用
	static constexpr float EXIT_HEIGHT = -10.0f;

#pragma region 参照値

	const MSG_SENDER_ID& operatorSenderId;
	const CharacterStats& stats;

#pragma endregion

	// 複製用ハンドル
	int bombModel;
	int backModel;
	int frontModel;

	// 武器の配列
	BombPoolEntry weapons[ArraySum(WeaponPoolCounts, (int)WeaponType::Max)];
};
