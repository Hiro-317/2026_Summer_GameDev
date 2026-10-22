#include "GrapePlayerWeaponThrow.h"

#include "../../../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../../../Manager/TimeScale/TimeScale.h"

#include "../../../../../../Common/Collider/SphereCollider.h"
#include "../../../../../../Common/CharacterBase/CharacterStatsDefine.h"

GrapePlayerWeaponThrow::GrapePlayerWeaponThrow(
	short ATTACK_RATE_PERCENT,
	const CharacterStats& playerStats,

	const Transform& playerTrans,

	const MSG_SENDER_ID& controlSenderId
) :
	ActorBase(),

	ATTACK_RATE_PERCENT(ATTACK_RATE_PERCENT),
	playerStats(playerStats),
	playerTrans(playerTrans),
	controlSenderId(controlSenderId),

	moveVec(),
	lifeCounter(),
	state()
{
}

void GrapePlayerWeaponThrow::Load(void)
{
#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(true);

	// 衝突時の押し出しを無効にする
	SetPushFlg(true);

#pragma endregion

	// 弾のモデルをロード、スケールの設定
	trans.LoadModel("Character/Grape/Bomb");
	trans.scale = 0.1f;

	// コライダー生成
	SphereCollider* coll = new SphereCollider(COLLIDER_TAG::PlayerAttack, 50.0f);
	coll->SetSkillStats(new SkillStats(ATTACK_RATE_PERCENT, &playerStats));
	AddCollider(coll);
	coll = nullptr;

	// 初期化処理
	SetJudgeFlg(false);

	// 最初は描画しない
	SetIsDraw(false);

	state = STATE::None;
}

void GrapePlayerWeaponThrow::SubUpdate()
{
	switch (state) {

	case GrapePlayerWeaponThrow::STATE::None: { return; }

	case GrapePlayerWeaponThrow::STATE::Move: {

		// 撃った瞬間から弾の生存時間を減らす
		lifeCounter -= TimeScale::Get();
		if (lifeCounter < 0.0f) {

			lifeCounter = 0.0f;

			Blast();

			return;
		}

		// 弾を飛ばす処理
		trans.pos += moveVec;

		break;
	}

	case GrapePlayerWeaponThrow::STATE::Blast: {

		if (GetJudgeFlg()) { SetJudgeFlg(false); }

		state = STATE::None;
		SetIsDraw(false);

		break;
	}

	}
}

void GrapePlayerWeaponThrow::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (!Net::GetIns().IsHost()) { return; }

	if (state == STATE::None) { return; }

	// 攻撃の当たり判定
	switch (other.GetTag()) {

	case COLLIDER_TAG::Enemy:
	case COLLIDER_TAG::BossDistance:
		Blast();
		break;

	default:break;
	}
}

void GrapePlayerWeaponThrow::Shot(const Vector3& pos, const Vector3& vec, bool send)
{
	trans.pos = pos;
	moveVec = vec * 40.0f;
	velocity.y = 40.0f;

	state = STATE::Move;

	// 重力を無効にする
	SetGravityFlg(true);
	// 衝突時の押し出しを無効にする
	SetPushFlg(true);

	SetJudgeFlg(true);
	SetIsDraw(true);

	if (send) {
		Net::GetIns().Send(MsgDataGrapePlayerShot(pos, vec), controlSenderId);
	}
}

void GrapePlayerWeaponThrow::Blast(void)
{
	Blast(trans.pos, true);
}

void GrapePlayerWeaponThrow::Blast(const Vector3& pos, bool send)
{
	trans.pos = pos;

	state = STATE::Blast;

	// 重力を無効にする
	SetGravityFlg(false);
	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

	if (send) {
		Net::GetIns().Send(MsgDataGrapePlayerShotBlast(trans.pos), controlSenderId);
	}
}

void GrapePlayerWeaponThrow::ReceptionUpdate(void)
{
	while (MsgDataGrapePlayerThrow* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerThrow>(controlSenderId)) {

		Shot(dataPtr->pos, dataPtr->vec);

		delete dataPtr;
	}

	while (MsgDataGrapePlayerThrowBlast* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerThrowBlast>(controlSenderId)) {

		Blast(dataPtr->pos);

		delete dataPtr;
	}

	// 座標・角度の同期
	while (MsgDataGrapePlayerThrowPos* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerThrowPos>(controlSenderId)) {
		// 自分のキャラ（操作対象）の場合
		if (IsOwnControl()) {
			// ホストから送られた座標と今の自分の座標の距離を測る
			float diff = (trans.pos - dataPtr->pos).Length();

			// 誤差が小さいなら無視
			if (diff > 0.5f) {
				// 誤差が大きい場合、少しずつホストから送られた座標に寄せる（補間）
				trans.pos = trans.pos * 0.9f + dataPtr->pos * 0.1f;
			}

		}
		// 他人のキャラなら、そのまま同期
		else {
			trans.pos = dataPtr->pos;
		}


		delete dataPtr;
	}
}

void GrapePlayerWeaponThrow::SendUpdate(void)
{
	if (Net::GetIns().IsHost() || IsOwnControl()) {
		// 自身の座標
		Net::GetIns().Send(MsgDataGrapePlayerThrowPos(trans.pos), controlSenderId);
	}
}
