#pragma once
#include "../../../../../../Common/ActorBase/ActorBase.h"
#include "../../../../../../Common/CharacterBase/CharacterStatsDefine.h"
#include "../../../../../../Common/Collider/CapsuleCollider.h"


struct Transform;

class BananaBossBanadrill : public ActorBase
{
public:
	BananaBossBanadrill(const int& model, const int& front, const int& back, const CharacterStats& stats);
	~BananaBossBanadrill() = default;

	void Load(void) override;

	bool Live(void) { return live; }
	void SetLive(bool flg) { live = flg; }

	bool Boot(void) { return boot; }

	void SetPos(const Vector3& pos) { trans.pos = Vector3(pos.x, HEIGHT, pos.z); front.pos = Vector3::XZonly(pos.x, pos.z); back.pos = Vector3::XZonly(pos.x, pos.z); }

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	bool GetIsGround(void) { return isGround; }

private:

	void SubDraw(void) override;
	void SubUpdate(void) override;
	void SubRelease(void) override;

	static constexpr float HEIGHT = 1000.0f;
	static constexpr float VIEW_HEIGHT = 37.0f;

	static constexpr int ATTACK_DURATION = 45;
	static constexpr int BOOM_DURATION = 3;

	static constexpr int BOOM_CNT = 900;

	static constexpr float HP =  150.0f;

	const CharacterStats& stats;

	static constexpr float RADIUS = 100.0f;

	Transform front;
	Transform back;

	CapsuleCollider* collRef;
	CapsuleCollider* attCollRef;

	bool live;
	bool boot;
	bool prevLive;

	bool attack;
	bool first;
	int cnt;

	float hp;
};
