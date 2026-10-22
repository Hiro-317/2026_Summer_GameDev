#pragma once

#include "../../../../Common/ActorBase/ActorBase.h"

struct CharacterStats;

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
	30
};

class GrapeBossWeaponBase : public ActorBase
{
public:
		
	GrapeBossWeaponBase(int model, int front, int back);
	~GrapeBossWeaponBase() = default;

	// 規定用のLoad
	virtual void Load(void) override {};
	// 行うロード
	virtual void Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats) = 0;

	// 攻撃が終わったか
	virtual bool IsEnd(void)const { return state == STATE::None; }

	// 予測線の設定(円形)
	void SetViewScaleCircle(float scale) { collFront.scale = scale; };

	void SetViewPosCircle(void) {

		collBack.pos = trans.pos;
		collBack.pos.y = VIEW_HEIGHT;

		collFront.pos = trans.pos;
		collFront.pos.y = VIEW_HEIGHT;
	};


protected:

	enum class STATE {
		None,

		Fall,

		Ground,

		Blast,
	};

	// 基本
	void SubDraw(void) override;

	// 予測線
	Transform collBack;
	Transform collFront;

	STATE state;

	// 行動カウント
	float count;

private:
#pragma region 定数定義

	static constexpr float VIEW_HEIGHT = 37.0f;

#pragma endregion
};
