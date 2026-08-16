#include "BossSelectScene.h"

#include "../SceneManager/SceneManager.h"
#include "../ObjectUseDefine.h"

#include "../../Manager/Camera/FollowRemote/FollowRemoteCamera.h"
#include "../../Manager/Camera/Free/FreeCamera.h"

#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/BossSelect/BossSelectStage.h"
#include "../../Object/BossSelect/SkyShip/BossSelectSkyShip.h"

#include "../../Object/BossSelect/Spot/BossSelectSpotManager.h"

BossSelectScene::BossSelectScene() :
	SceneBase()
{
}

void BossSelectScene::SubPostLoad(void)
{
	ObjAdd(new SkyDome(200.0f));
	ObjAdd(new BossSelectStage());
	ObjAdd(new BossSelectSpotManager());
	ObjAdd(new BossSelectSkyShip());
}

void BossSelectScene::SubPostInit(void)
{

}

void BossSelectScene::CreateCamera(void)
{
	// í«è]ÉJÉÅÉâê∂ê¨
	camera = new FollowRemoteCamera(
		&ObjSerch<BossSelectSkyShip>(objects)->GetTrans().pos,
		Vector3::YZonly(500.0f, -500.0f),
		Vector3::Zonly(500.0f),
		0.0f
	);
}
