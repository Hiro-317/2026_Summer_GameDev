#include "GrapePlayerWeaponThrow.h"

#include <cmath>

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
    lifeCounter(0.0f),
    state(STATE::None)
{
    ActorBase::controlSenderId = controlSenderId;
}

void GrapePlayerWeaponThrow::Load(void)
{
    SetDynamicFlg(true);
    SetGravityFlg(false);
    SetPushFlg(false);
    trans.LoadModel("Character/Grape/Bomb");
    trans.scale = MODEL_SCALE;

    // 攻撃判定では敵を押し出さず、命中通知だけを受け取る。
    auto* attack = new SphereCollider(COLLIDER_TAG::PlayerAttack, COLLIDER_RADIUS);
    attack->SetSkillStats(new SkillStats(ATTACK_RATE_PERCENT, &playerStats));
    attack->SetPushFlg(false);
    AddCollider(attack);

    // 既存の地形専用グループを使い、地面への押し出しと接地通知を受け取る。
    auto* terrain = new SphereCollider(COLLIDER_TAG::BossAttackArea, COLLIDER_RADIUS);
    terrain->SetPushFlg(true);
    AddCollider(terrain);

    SetJudgeFlg(false);
    SetIsDraw(false);
}

void GrapePlayerWeaponThrow::SubUpdate(void)
{
    if (Net::GetIns().GetState() != Net::NetState::None) { ReceptionUpdate(); }
    if (state != STATE::Move) { return; }
    if (lifeCounter <= 0.0f) {
        Blast(trans.pos, Net::GetIns().IsHost());
        return;
    }

    // 水平の進行は維持し、専用の弱い重力で大きくゆっくり跳ねる。
    velocity.y -= BOUNCE_GRAVITY * TimeScale::Get();
    trans.pos += moveVec * TimeScale::Get();
    lifeCounter -= TimeScale::Get();
    SendUpdate();
}

void GrapePlayerWeaponThrow::SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other)
{
    if (state != STATE::Move || other.GetTag() != COLLIDER_TAG::Stage || velocity.y >= 0.0f) { return; }

    // 複数の地形に同時に触れても一度だけ反射する。小さい跳ね返りも維持する。
    const float rebound = -velocity.y * 0.85f;
    velocity.y = rebound > BOUNCE_SPEED ? rebound : BOUNCE_SPEED;
}

void GrapePlayerWeaponThrow::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
    if (!Net::GetIns().IsHost() || state != STATE::Move || ownTag != COLLIDER_TAG::PlayerAttack) { return; }
    if (other.GetTag() == COLLIDER_TAG::Enemy || other.GetTag() == COLLIDER_TAG::BossDistance) { Blast(); }
}

void GrapePlayerWeaponThrow::Shot(const Vector3& pos, const Vector3& vec, bool send)
{
    Vector3 direction(vec.x, 0.0f, vec.z);
    if (direction.LengthSq() <= 0.000001f) { return; }
    direction.Normalize();
    trans.pos = pos + Vector3::Yonly(COLLIDER_RADIUS - 50.0f);
    trans.prevPos = trans.pos;
    trans.SetRotation(Quaternion::FromRotationY(std::atan2(direction.x, direction.z)));
    moveVec = direction * 10.0f;
    velocity = Vector3::Yonly(BOUNCE_SPEED);
    lifeCounter = static_cast<float>(LIFE_TIME);
    state = STATE::Move;
    SetGravityFlg(false);
    SetPushFlg(true);
    SetJudgeFlg(true);
    SetIsDraw(true);

    if (send) { Net::GetIns().Send(MsgDataGrapePlayerThrow(pos, direction), controlSenderId); }
}

void GrapePlayerWeaponThrow::Blast(void)
{
    Blast(trans.pos, true);
}

void GrapePlayerWeaponThrow::Blast(const Vector3& pos, bool send)
{
    trans.pos = pos;
    moveVec = Vector3();
    velocity = Vector3();
    lifeCounter = 0.0f;
    state = STATE::None;
    SetGravityFlg(false);
    SetPushFlg(false);
    SetJudgeFlg(false);
    SetIsDraw(false);
    if (send) { Net::GetIns().Send(MsgDataGrapePlayerThrowBlast(pos), controlSenderId); }
}

void GrapePlayerWeaponThrow::ReceptionUpdate(void)
{
	while (MsgDataGrapePlayerThrow* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerThrow>(controlSenderId, true)) {

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
	if (state == STATE::Move && Net::GetIns().IsHost()) {
		// 自身の座標
		Net::GetIns().Send(MsgDataGrapePlayerThrowPos(trans.pos), controlSenderId);
	}
}
