#include "BananaBossBanadrill.h"

#include "../../../../../../../Manager/Sound/SoundManager.h"

#include "../../../../../../Common/Transform/Transform.h"
#include "../../../../../../Common/Effect/ExplosionActor.h"
#include "../../../../../../Common/Effect/DustBurstActor.h"


BananaBossBanadrill::BananaBossBanadrill(const int& model, const int& front, const int& back, const CharacterStats& stats) :
	ActorBase(),
	stats(stats)
{
	trans.model = model;
	this->front.model = front;
	this->back.model = back;
}

void BananaBossBanadrill::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	collRef = new SphereCollider(COLLIDER_TAG::Boss, RADIUS);
	attCollRef = new XZCircleCollider(COLLIDER_TAG::BossAttack, DRILL_RADIUS, DRILL_RADIUS);
	AddCollider(collRef);
	AddCollider(attCollRef);

	SetPushWeight(255);

	attCollRef->SetSkillStats(new SkillStats(85, &stats));
	attCollRef->SetJudgeFlg(false);

	collRef->SetJudgeFlg(false);

	trans.pos = Vector3(HEIGHT);

	trans.scale = Vector3(8.0f);
	front.scale = Vector3();
	back.scale = Vector3(DRILL_RADIUS / 100.0f);

	front.pos = Vector3::Yonly(VIEW_HEIGHT);
	back.pos = Vector3::Yonly(VIEW_HEIGHT);
	first = true;

	SetGravityFlg(true);

	SetIsDraw(false);

	SetDrawType(ACTOR_DRAW_TYPE::Alpha);

	hp = HP;
	boot = false;

	exRef = new ExplosionActor(trans.pos, 350.0f, 1.2f, 1.2f, 1.0f, 0xffff00);
	AddChildActor(exRef);

	DustBurstSettings set;
	set.durationSeconds = 0.0f;
	set.color = 0xffff99;
	set.height = RADIUS;

	duRef = new DustBurstActor(trans.pos, set);
	AddChildActor(duRef);
}

void BananaBossBanadrill::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (!attack && live) {
		if (other.GetTag() == COLLIDER_TAG::PlayerAttack && ownTag == COLLIDER_TAG::Boss) {
			// 防御力も含めたダメージを取得
			short damage = CalculateDamage(other.GetSkillStats()->Power(), stats.defensePower.Value());

			// ダメージ減らす
			hp -= damage;

			if (hp <= 0) {
				live = false;
				prevLive = false;
				hp = HP;
				SetIsDraw(false);
				collRef->SetJudgeFlg(false);
			}
		}
	}
}

void BananaBossBanadrill::EffectUpdate(void)
{
	if (!exRef->IsFinished()) {
		exRef->Update();
	}
	if (!duRef->IsFinished()) {
		duRef->Update();
	}
}

void BananaBossBanadrill::SubDraw(void)
{
	SetUseLighting(false);
	back.Draw();
	front.Draw();
	SetUseLighting(true);
}

void BananaBossBanadrill::SubUpdate(void)
{
	if (!prevLive && live) {
		first = true;
		prevLive = live;
		SetIsDraw(true);
		attack = true;
		collRef->SetJudgeFlg(true);
		hp = HP;
		attCollRef->SetSkillStats(new SkillStats(85, &stats));
		attCollRef->SetRadius(DRILL_RADIUS);
		back.scale = Vector3(DRILL_RADIUS / 100.0f);
		cnt = 0;
	}
	// 地面についてないなら加速して落ちる
	if (!isGround) {

		front.scale = Vector3((DRILL_RADIUS / 100.0f) * ((HEIGHT - trans.pos.y) / (HEIGHT - VIEW_HEIGHT - RADIUS)));
		trans.pos.y -= 3.0f;
		trans.AddAngle(Vector3::Yonly(Deg2Rad(25.0f)));
	}
	// ついてるなら爆発カウントを進める
	else {
		// 前フレームの判定が残ってるので
		// それ防止に
		// 開始されたフレームの検知
		if (first) {
			// 今開始されたらフラグを立てる
			first = false;
			// ほんとはついてないので落下
			front.scale = Vector3((DRILL_RADIUS / 100.0f) * ((HEIGHT - trans.pos.y) / (HEIGHT - VIEW_HEIGHT - RADIUS)));
			trans.pos.y -= 3.0f;
			trans.AddAngle(Vector3::Yonly(Deg2Rad(5.0f)));
			return;
		}
		if(attack){
			if (cnt == 0) {
				boot = true;
				duRef->Restart();
				duRef->SetPos(Vector3(trans.pos.x, trans.pos.y - RADIUS, trans.pos.z));
				Snd::GetIns().Play("Drill");
			}
			cnt++;
			// 攻撃判定オン
			if (ATTACK_DURATION >= cnt) {
				attCollRef->SetJudgeFlg(true);
				trans.AddAngle(Vector3::Yonly(Deg2Rad(25.0f * ((float)(ATTACK_DURATION - cnt) / (float)ATTACK_DURATION))));
			}
			//攻撃判定オフ、終わり
			else {
				attCollRef->SetJudgeFlg(false);
				attack = false;
				cnt = 0;
				front.scale = Vector3(0.0f);
				duRef->Stop();
				Snd::GetIns().Stop("Drill");
			}
		}
		else {
			if (cnt == 0) {
				back.scale = Vector3(BOOM_RADIUS / 100.0f);
				attCollRef->SetRadius(BOOM_RADIUS);
				attCollRef->SetSkillStats(new SkillStats(150, &stats));
			}
			cnt++;
			if (BOOM_CNT > cnt) {
				front.scale = Vector3((BOOM_RADIUS / 100.0f) * ((float)cnt / (float)BOOM_CNT));
			}
			if (BOOM_CNT == cnt) {
				front.scale = Vector3(BOOM_RADIUS / 100.0f);
				attCollRef->SetJudgeFlg(true);
				exRef->Play();
				exRef->SetPos(trans.pos);
				Snd::GetIns().Play("ExBanana");
			}
			if (BOOM_CNT + BOOM_DURATION <= cnt) {
				attCollRef->SetJudgeFlg(false);
				collRef->SetJudgeFlg(false);
				prevLive = false;
				live = false;
				cnt = 0;
				front.scale = Vector3(0.0f);
				SetIsDraw(false);
			}
		}
	}
}

void BananaBossBanadrill::SubRelease(void)
{
	back.Release();
	front.Release();
}
