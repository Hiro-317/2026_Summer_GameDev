#pragma once

#include "../../../../Common/ActorBase/ActorBase.h"

#include "../../../../Common/Effect/ExplosionActor.h"


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
	void Load(void) override {};
	// 行うロード
	virtual void Load(const CharacterStats& stats) = 0;

	// 初期設定用関数
	void SetStartPos(const Vector3& pos) { trans.pos = pos; }
	void ResetCount(void) { count = 0.0f; }

	// コライダーのオン/オフ
	virtual void SetColliderFlg(bool flg);
	void ResetEnd(void) { end = false; }

	/// <summary>
	/// 弾本体の更新が停止している間、子の爆発エフェクトだけを更新する。
	/// </summary>
	/// <remarks>生存中はActorBase::Updateが子を更新するため、重ねて呼ばない。</remarks>
	void UpdateInactiveEffects(void);

	// 攻撃が終わったか
	bool IsEnd(void) { return end; }

	// 予測線の設定(円形)
	void SetViewScaleCircle(float scale) { collFront.scale = Vector3(scale); };
	void SetViewPosCircle(void) { collBack.pos = trans.pos; collFront.pos = trans.pos; collBack.pos.y = VIEW_HEIGHT; collFront.pos.y = VIEW_HEIGHT; };

protected:

	/// <summary>
	/// 爆発攻撃の判定を有効化し、最初の1フレームだけ着弾位置でエフェクトを再生する。
	/// </summary>
	/// <remarks>攻撃中に毎フレーム呼んでもPlayは繰り返さない。</remarks>
	void BeginExplosionAttack(void);

	// 基本
	void SubDraw(void) override;

	// 予測線
	Transform collBack;
	Transform collFront;

	// 行動カウント
	float count;

	// 終了フラグ
	bool end;

	// 今開始されたか
	bool now;

	// AddChildActorが所有する爆発アクターへの参照。爆発を持たない武器ではnullptr。
	ExplosionActor* insRef;
private:
#pragma region 定数定義

	static constexpr float VIEW_HEIGHT = 37.0f;

#pragma endregion
};
