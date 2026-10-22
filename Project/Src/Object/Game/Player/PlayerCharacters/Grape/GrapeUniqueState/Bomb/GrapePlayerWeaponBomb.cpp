#include "GrapePlayerWeaponBomb.h"

#include "../../../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../../../Manager/TimeScale/TimeScale.h"

#include "../../../../../../Common/Collider/SphereCollider.h"
#include "../../../../../../Common/CharacterBase/CharacterStatsDefine.h"

GrapePlayerWeaponBomb::GrapePlayerWeaponBomb(
	short ATTACK_RATE_PERCENT,
	const CharacterStats& playerStats,

	float ATTACK_COUNT_TIME,

	const Transform& playerTrans,

	const MSG_SENDER_ID& controlSenderId
) :
	ActorBase(),

	ATTACK_RATE_PERCENT(ATTACK_RATE_PERCENT),
	playerStats(playerStats),
	ATTACK_COUNT_TIME(ATTACK_COUNT_TIME),
	playerTrans(playerTrans),
	controlSenderId(controlSenderId),

	lifeCounter(),
	state(),
	collRef(nullptr)
{
}

void GrapePlayerWeaponBomb::Load(void)
{
#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 衝突時の押し出しを無効にする
	SetPushFlg(true);

	SetPushWeight(20);

#pragma endregion

	trans.LoadModel("Character/Grape/Bomb");
	trans.scale = 0.5f;

	// コライダー生成
	collRef = new SphereCollider(COLLIDER_TAG::PlayerAttack, PUT_RADIUS);
	// スキルのダメージ量の設定
	collRef->SetSkillStats(new SkillStats(ATTACK_RATE_PERCENT, &playerStats));
	AddCollider(collRef);

	// 初期化処理
	SetJudgeFlg(false);

	// 最初は描画しない
	SetIsDraw(false);
}

void GrapePlayerWeaponBomb::SubUpdate(void)
{
	switch (state) {

	case GrapePlayerWeaponBomb::STATE::None: { return; }

	case GrapePlayerWeaponBomb::STATE::Put: {

		// 撃った瞬間から弾の生存時間を減らす
		lifeCounter -= TimeScale::Get();
		if (lifeCounter < 0.0f) {

			lifeCounter = 0.0f;

			Blast();

			return;
		}

		break;
	}

	case GrapePlayerWeaponBomb::STATE::Blast: {

		if (GetJudgeFlg()) { SetJudgeFlg(false); }

		state = STATE::None;
		SetIsDraw(false);

		break;
	}

	}
}

void GrapePlayerWeaponBomb::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (!Net::GetIns().IsHost()) { return; }
}

void GrapePlayerWeaponBomb::Put(const Vector3& pos, bool send)
{
	trans.pos = pos;
	velocity = playerTrans.rotation.Rotate(Vector3(0, 20, 5));

	state = STATE::Put;

	SetJudgeFlg(true);
	SetIsDraw(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 衝突時の押し出しを無効にする
	SetPushFlg(true);

	collRef->SetRadius(PUT_RADIUS * trans.scale.MaxElementF());

	if (send) {
		Net::GetIns().Send(MsgDataGrapePlayerBomb(pos), controlSenderId);
	}
}

void GrapePlayerWeaponBomb::Blast(void)
{
	Blast(trans.pos, true);
}

void GrapePlayerWeaponBomb::Blast(const Vector3& pos, bool send)
{
	trans.pos = pos;

	state = STATE::Blast;

	// 重力を有効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

	collRef->SetRadius(BLAST_RADIUS * trans.scale.MaxElementF());

	if (send) {
		Net::GetIns().Send(MsgDataGrapePlayerShotBlast(trans.pos), controlSenderId);
	}
}

void GrapePlayerWeaponBomb::ReceptionUpdate(void)
{
	while (MsgDataGrapePlayerShot* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerShot>(controlSenderId)) {

		Put(dataPtr->pos);

		delete dataPtr;
	}

	while (MsgDataGrapePlayerShotBlast* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerShotBlast>(controlSenderId)) {

		Blast(dataPtr->pos);

		delete dataPtr;
	}
}