#include "TomatoBossStage.h"

#include"../../Common/Collider/BoxCollider.h"
#include"../../Common/Collider/SphereCollider.h"

TomatoBossStage::TomatoBossStage() 
{
}

void TomatoBossStage::Load(void)
{
	trans.Load("Stage/TomatoBoss/TomatoBossStage");

	ColliderCreate(new BoxCollider(TAG::STAGE, GROUND_COLLISION_SIZE));
	ColliderCreate(new SphereCollider(TAG::STAGE, 150, -1.0f, Vector3(1241.92f, 130.6f + MODEL_CENTER_DIFF.y, -341.64f)));

	trans.centerDiff = MODEL_CENTER_DIFF;
}


