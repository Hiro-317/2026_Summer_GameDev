#include "TomatoBossStage.h"

#include"../../Common/Collider/BoxCollider.h"
#include"../../Common/Collider/CapsuleCollider.h"

TomatoBossStage::TomatoBossStage() 
{
}

void TomatoBossStage::Load(void)
{
	trans.Load("Stage/TomatoBoss/TomatoBossStage");

	trans.centerDiff = MODEL_CENTER_DIFF;

	ColliderLoad();
}

void TomatoBossStage::ColliderLoad()
{
	// 地面の当たり判定
	ColliderCreate(new BoxCollider(TAG::STAGE, GROUND_COLLISION_SIZE));

	// ステージの岩の当たり判定
	for (int i = 0; i < ROCK_NUM; i++)
	{
		ColliderCreate(new CapsuleCollider(TAG::STAGE, Vector3(0.0f), Vector3::Yonly(100.0f), ROCK_COLLISION_INFO[i].radius, -1.0f, ROCK_COLLISION_INFO[i].pos));
	}
}


