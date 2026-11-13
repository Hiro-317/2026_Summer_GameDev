#include "GrapePlayerWeaponShot.h"

#include <algorithm>
#include <cmath>

#include "../../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../../Manager/TimeScale/TimeScale.h"
#include "../../../../../../Common/Collider/SphereCollider.h"
#include "../../../../../../Common/CharacterBase/CharacterStatsDefine.h"
#include "../../../../../../Common/Effect/ExplosionActor.h"

GrapePlayerWeaponShot::GrapePlayerWeaponShot(
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
    state(STATE::None),
    explosionPosition(),
    explosion(nullptr),
    hasExploded(false)
{
    // 所有プレイヤーと同じ操作権・ダメージ送信者IDを武器にも設定する。
    ActorBase::controlSenderId = controlSenderId;
}

void GrapePlayerWeaponShot::Load(void)
{
    SetDynamicFlg(true);
    SetGravityFlg(false);
    SetPushFlg(false);
    trans.LoadModel("Character/Grape/Bomb");
    trans.scale = 0.1f;

    auto* coll = new SphereCollider(COLLIDER_TAG::PlayerAttack, 50.0f);
    coll->SetSkillStats(new SkillStats(ATTACK_RATE_PERCENT, &playerStats));
    AddCollider(coll);
    // GrapeBossと同じ既定の色・大きさ・寿命で準備する。
    explosion = new ExplosionActor(explosionPosition);
    AddChildActor(explosion);

    SetJudgeFlg(false);
    SetIsDraw(false);
    state = STATE::None;
}

void GrapePlayerWeaponShot::SubUpdate(void)
{
    // 発射と消滅の通知は、この武器が自身で受け取る。
    if (Net::GetIns().GetState() != Net::NetState::None) { ReceptionUpdate(); }
    if (state != STATE::Move) { return; }

    if (lifeCounter <= 0.0f) {
        // スキル1の寿命切れは爆発なしで消す。命中時だけBlast通知で再生する。
        moveVec = Vector3();
        velocity = Vector3();
        state = STATE::None;
        SetJudgeFlg(false);
        SetIsDraw(false);
        return;
    }

    // 保存済みの速度だけで移動し、発射後のプレイヤーには追従しない。
    const float elapsed = (std::min)(TimeScale::Get(), lifeCounter);
    trans.pos += moveVec * elapsed;
    lifeCounter -= elapsed;
    SendUpdate();
}

void GrapePlayerWeaponShot::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
    if (!Net::GetIns().IsHost() || state != STATE::Move) { return; }
    // ボス本体の押し出し判定ではなく、ダメージ受付判定への命中で終了する。
    if (other.GetTag() == COLLIDER_TAG::Enemy || other.GetTag() == COLLIDER_TAG::BossDistance) {
        Blast();
    }
}

void GrapePlayerWeaponShot::Shot(const Vector3& pos, const Vector3& vec, bool send)
{
    if (vec.LengthSq() <= 0.000001f) { return; }
    const Vector3 direction = vec.Normalized();
    trans.pos = pos;
    trans.prevPos = pos;
    trans.SetRotation(Quaternion::FromRotationY(std::atan2(direction.x, direction.z)));
    moveVec = direction * 40.0f;
    velocity = Vector3();
    lifeCounter = static_cast<float>(LIFE_TIME);
    state = STATE::Move;
    hasExploded = false;
    SetJudgeFlg(true);
    SetIsDraw(true);

    if (send) { Net::GetIns().Send(MsgDataGrapePlayerShot(pos, direction), controlSenderId); }
}

void GrapePlayerWeaponShot::Blast(void)
{
    Blast(trans.pos, true);
}

void GrapePlayerWeaponShot::Blast(const Vector3& pos, bool send)
{
    // 寿命による再生と受信通知が重なっても、同じ爆発を再開始しない。
    if (hasExploded) { return; }
    hasExploded = true;
    // 消える瞬間の座標で一度再生する。子Actorなので本体が非表示でも更新される。
    explosionPosition = pos;
    explosion->SetPos(explosionPosition);
    explosion->Restart();

    // 命中時に判定と描画を止め、次の発射に再利用する。
    trans.pos = pos;
    moveVec = Vector3();
    lifeCounter = 0.0f;
    state = STATE::None;
    SetJudgeFlg(false);
    SetIsDraw(false);
    if (send) { Net::GetIns().Send(MsgDataGrapePlayerShotBlast(pos), controlSenderId); }
}

void GrapePlayerWeaponShot::ReceptionUpdate(void)
{
	while (MsgDataGrapePlayerShot* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerShot>(controlSenderId, true)) {

		Shot(dataPtr->pos, dataPtr->vec);

		delete dataPtr;
	}

	while (MsgDataGrapePlayerShotBlast* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerShotBlast>(controlSenderId)) {

		Blast(dataPtr->pos);

		delete dataPtr;
	}

	// 座標・角度の同期
	while (MsgDataGrapePlayerShotPos* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerShotPos>(controlSenderId)) {
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

void GrapePlayerWeaponShot::SendUpdate(void)
{
	if (state == STATE::Move && Net::GetIns().IsHost()) {
		// 自身の座標
		Net::GetIns().Send(MsgDataGrapePlayerShotPos(trans.pos), controlSenderId);
	}
}
