#include "BossSelectSpotArea.h"

#include "../../../Scene/SceneManager.h"

#include "../../Common/Collider/XZCircleCollider.h"

BossSelectSpotArea::BossSelectSpotArea(float COLLIDER_RADIUS, BOSS_TYPE bossType, bool& selected, const Transform& trans) :
	ActorBase(),

	COLLIDER_RADIUS(COLLIDER_RADIUS + COLLIDER_RADIUS_EXTRA),
	bossType(bossType),
	selected(selected)
{
	this->trans.pos = trans.pos;
	this->trans.rotation = trans.rotation;
}

void BossSelectSpotArea::Load(void)
{
	// 静的オブジェクトに設定
	SetDynamicFlg(false);
	// オブジェクト設定の押し出しは無効
	SetPushFlg(false);

	// 選択判定（飛行船が近くにいるか）を行うコライダー
	AddCollider(new XZCircleCollider(COLLIDER_TAG::Boss, COLLIDER_RADIUS, 10000.0f));
}

void BossSelectSpotArea::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (other.GetTag() == SKY_SHIP_COLL_TAG) {
		SceneManager::GetIns().SetSelectBossType(bossType);

		selected = true;
	}
}