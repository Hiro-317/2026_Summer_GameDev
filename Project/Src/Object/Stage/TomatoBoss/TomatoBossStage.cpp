#include "TomatoBossStage.h"

#include"../../Common/Collider/BoxCollider.h"
#include"../../Common/Collider/CapsuleCollider.h"
#include"../../Common/Collider/SphereCollider.h"

TomatoBossStage::TomatoBossStage() :
	ActorBase("Data/Parameter/Stage/TomatoStageParameter.csv")
{
}

void TomatoBossStage::Load(void)
{
	trans.Load("Stage/TomatoBoss/TomatoBossStage");

	// ステージのモデルの座標の補正
	trans.centerDiff = MODEL_CENTER_DIFF;
	
	// 衝突判定の情報読み込み
	ColliderLoad();
}

void TomatoBossStage::ColliderLoad()
{
	// 地面の当たり判定
	ColliderCreate(new BoxCollider(TAG::STAGE, GROUND_COLLISION_SIZE));

	//// ステージの岩の当たり判定
	for (const ColliderInfo& info : ROCK_COLLISION_INFO) {
		ColliderCreate(new CapsuleCollider(TAG::STAGE, Vector3(0.0f), ROCK_LOCAL_ENDPOS, info.radius, -1.0f, info.pos));
	}

	// ステージの壁の当たり判定
	for (const ColliderInfo& info : WALL_COLLISION_INFO) {
		ColliderCreate(new CapsuleCollider(TAG::STAGE, Vector3(0.0f), WALL_LOCAL_ENDPOS, info.radius, -1.0f, info.pos));
	}
}


