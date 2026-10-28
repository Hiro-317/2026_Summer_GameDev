#include "GrapeBossStage.h"

#include "../../../Common/Collider/BoxCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/SphereCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"

GrapeBossStage::GrapeBossStage() : ActorBase("Data/Parameter/Stage/Grape/")
{
}

void GrapeBossStage::Load(void)
{
	trans.LoadModel("Stage/GrapeBoss/GrapeBossStage");

	// ステージのモデルの座標の補正
	trans.centerDiff = MODEL_CENTER_DIFF;
	trans.scale = Vector3(4.0f);

#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を有効にする
	SetDynamicFlg(false);

	// 衝突時の押し出しを有効にする
	SetPushFlg(true);

#pragma endregion
	
	// 衝突判定の情報読み込み
	ColliderLoad();
}

void GrapeBossStage::ColliderLoad()
{
	// 地面の当たり判定
	AddCollider(new BoxCollider(COLLIDER_TAG::Stage, GROUND_COLLISION_SIZE));

	// ステージの壁の当たり判定
	for (const ColliderInfo& info : WALL_COLLISION_INFO) {
		AddCollider(
			new BoxCollider(
				COLLIDER_TAG::Stage,
				info.size,
				info.pos,
				Quaternion::FromEulerXZY(Vector3::Yonly(info.yAngle))
			)
		);
	}
}


