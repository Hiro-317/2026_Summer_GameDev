#pragma once
#include "../../../../../../Common/ActorBase/ActorBase.h"
#include "../../../../../../Common/CharacterBase/CharacterStatsDefine.h"
#include "../../../../../../Common/Collider/XZCircleCollider.h"
#include "../../../../../../Common/Collider/SphereCollider.h"


struct Transform;

class ExplosionActor;

class BananaBossBanadrill : public ActorBase
{
public:
	BananaBossBanadrill(const int& model, const int& front, const int& back, const CharacterStats& stats);
	~BananaBossBanadrill() = default;

	void Load(void) override;

	bool GetLive(void) { return live; }
	void SetLive(bool flg) { live = flg; }

	void SetPos(const Vector3& pos) { trans.pos = Vector3(pos.x, HEIGHT, pos.z); front.pos = Vector3(pos.x, VIEW_HEIGHT, pos.z); back.pos = Vector3(pos.x, VIEW_HEIGHT, pos.z); }

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	void EffectUpdate(void);

	bool GetBoot(void) const { return boot; }
	void ResetBoot(void) { boot = false; }

private:

	void SubDraw(void) override;
	void SubUpdate(void) override;
	void SubRelease(void) override;

	static constexpr float HEIGHT = 1000.0f;
	static constexpr float VIEW_HEIGHT = 37.0f;

	static constexpr int ATTACK_DURATION = 45;
	static constexpr int BOOM_DURATION = 3;

	static constexpr int BOOM_CNT = 300;

	static constexpr float HP =  100.0f;

	const CharacterStats& stats;

	static constexpr float RADIUS = 80.0f;
	static constexpr float DRILL_RADIUS = 150.0f;
	static constexpr float BOOM_RADIUS = 250.0f;

	Transform front;
	Transform back;

	SphereCollider* collRef;
	XZCircleCollider* attCollRef;

	bool live;
	bool prevLive;

	bool boot;

	bool attack;
	bool first;
	int cnt;

	float hp;

	ExplosionActor* exRef;
};
