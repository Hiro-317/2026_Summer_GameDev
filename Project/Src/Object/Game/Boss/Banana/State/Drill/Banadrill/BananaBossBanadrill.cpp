#include "BananaBossBanadrill.h"

#include "../../../../../../Common/Transform/Transform.h"


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
	collRef = new CapsuleCollider(COLLIDER_TAG::Boss, Vector3(), Vector3(), RADIUS);
	attCollRef = new CapsuleCollider(COLLIDER_TAG::BossAttack, Vector3(), Vector3(), RADIUS * 3.0f / 2.0f);
	AddCollider(collRef);
	AddCollider(attCollRef);

	attCollRef->SetSkillStats(new SkillStats(85, &stats));
	attCollRef->SetPushFlg(false);
	attCollRef->SetJudgeFlg(false);

	front.scale = Vector3();
	back.scale = Vector3(RADIUS / 100.0f);

	front.pos = Vector3::Yonly(VIEW_HEIGHT);
	back.pos = Vector3::Yonly(VIEW_HEIGHT);

	SetGravityFlg(true);

	SetDrawType(ACTOR_DRAW_TYPE::Alpha);
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
				hp = HP;
			}
		}
	}
}

void BananaBossBanadrill::SubDraw(void)
{
	if (attack) {
		SetUseLighting(false);
		back.Draw();
		front.Draw();
		SetUseLighting(true);
	}
}

void BananaBossBanadrill::SubUpdate(void)
{
	// 地面についてないなら加速して落ちる
	if (!isGround) {

		front.scale = Vector3((RADIUS / 100.0f) * ((HEIGHT - trans.pos.y) / (HEIGHT - VIEW_HEIGHT)));
		trans.pos.y -= 8.0f;
		trans.AddAngle(Vector3::Yonly(Deg2Rad(5.0f)));
	}
	// ついてるなら爆発カウントを進める
	else {
		// 前フレームの判定が残ってるので
		// それ防止に
		// 開始されたフレームの検知
		if (!attack) {
			// 今開始されたらフラグを立てる
			attack = true;
			// ほんとはついてないので落下
			front.scale = Vector3((RADIUS / 100.0f) * ((HEIGHT - trans.pos.y) / (HEIGHT - VIEW_HEIGHT)));
			trans.pos.y -= 8.0f;
			trans.AddAngle(Vector3::Yonly(Deg2Rad(5.0f)));
			return;
		}
		cnt++;
		// 攻撃判定オン
		if (ATTACK_DURATION >= cnt) {
			ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetJudgeFlg(true);
			trans.AddAngle(Vector3::Yonly(Deg2Rad(5.0f * ((ATTACK_DURATION - cnt) / ATTACK_DURATION))));
		}
		//攻撃判定オフ、終わり
		else {
			ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetJudgeFlg(false);
			attack = false;
			boot = true;
			cnt = 0;
			front.scale = Vector3(0.0f);
		}
	}
}

void BananaBossBanadrill::SubRelease(void)
{
	back.Release();
	front.Release();
}
