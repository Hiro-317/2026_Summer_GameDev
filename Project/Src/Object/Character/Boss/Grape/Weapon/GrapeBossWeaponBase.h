#pragma once
#include "../../../../ActorBase.h"


// 種類の列挙型定義
enum class WeaponType {
	None = -1,

	// 直線投擲
	Straight,

	// 爆弾投下（蹴り攻撃と同時に身の回りに落とす爆弾）
	KickBomb,

	// 爆弾投下（スタンプ攻撃と同時に身の回りに落とす爆弾）
	StampBomb,

	// 爆弾投下（プレイヤーに向かって降らせる爆弾）
	SingleBomb,

	// 爆弾投下（1体のプレイヤーに一定時間降らせ続ける爆弾）
	StalkerBomb,

	// 爆弾投下（ランダムに地上に大量投下する爆弾）
	RandomBomb,

	Max
};

static constexpr int WeponDuplicateNum[(int)WeaponType::Max] = {
	// 直線投擲
	3,

	// 爆弾投下（蹴り攻撃と同時に身の回りに落とす爆弾）
	7,

	// 爆弾投下（スタンプ攻撃と同時に身の回りに落とす爆弾）
	10,

	// 爆弾投下（プレイヤーに向かって降らせる爆弾）
	1,

	// 爆弾投下（1体のプレイヤーに一定時間降らせ続ける爆弾）
	5,

	// 爆弾投下（ランダムに地上に大量投下する爆弾）
	20
};

class GrapeBossWeaponBase : public ActorBase
{
public:

	GrapeBossWeaponBase(std::string parameterPath, int model) { trans.Duplicate(model);	}
	~GrapeBossWeaponBase() = default;

	// 規定用のLoad
	virtual void Load(void) override {};
	// 行うロード
	virtual void Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats);

protected:

	void SubDraw(void) override;
};
