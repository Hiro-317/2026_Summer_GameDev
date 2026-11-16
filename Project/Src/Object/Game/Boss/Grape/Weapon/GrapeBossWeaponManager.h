#pragma once
#include "GrapeBossWeaponBase.h"

#include "../../../../../Manager/Net/SenderID_Define.h"

struct CharacterStats;

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

	/// <summary>種類ごとの爆弾を事前生成し、再利用用の連続した領域に登録する。</summary>
	/// <param name="operatorSenderId">操作権の送信者IDへの参照。マネージャー利用中は有効であること。</param>
	/// <param name="stats">攻撃に使うボス能力値への参照。</param>
	GrapeBossWeaponManager(const MSG_SENDER_ID& operatorSenderId, const CharacterStats& stats);
	~GrapeBossWeaponManager() = default;

	/// <summary>指定種類の先頭要素を返す。該当なしの場合はnullptr。</summary>
	/// <param name="type">取得する爆弾の種類。</param>
	/// <returns>内部配列の借用ポインター。添字は該当種類のWeponDuplicateNum未満に制限する。</returns>
	BombType* GetWeapons(WeaponType type) {

		// 配列分出す
		for (auto& weapon : weapons) {
			// 配列内のタイプとほしいものが同じなら
			if (weapon.type == type) {
				// 格納
				return &weapon;
			}
		}
		// ないならぬるぽ
		return nullptr;
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
	BombType weapons[ArraySum(WeponDuplicateNum, (int)WeaponType::Max)];
};
