#include "BossSelectSpotManager.h"

#include "../../../Manager/Input/InputManager.h"

#include "Tomato/BossSelectTomatoSpot.h"
#include "Grape/BossSelectGrapeSpot.h"
#include "Banana/BossSelectBananaSpot.h"

BossSelectSpotManager::BossSelectSpotManager() :
	ActorBase(),

	spot(),
	selectedImage()
{
}

void BossSelectSpotManager::Load(void)
{
	selectedImage[(int)true] = LoadGraph("Data/Image/BossSelect/BossSelectEnterController.png");
	selectedImage[(int)false] = LoadGraph("Data/Image/BossSelect/BossSelectEnterKeyboard.png");

	// スポット生成ラムダ関数
	auto CreatSpot = [&](BossSelectSpotBase* newSpot)->void {

		// 安全処理
		if (newSpot == nullptr) { return; }

		// 追加
		spot.emplace_back(newSpot);
		AddChildActor(newSpot);
		};

	CreatSpot(new BossSelectTomatoSpot());
	CreatSpot(new BossSelectGrapeSpot());
	CreatSpot(new BossSelectBananaSpot());
}

void BossSelectSpotManager::SubDraw(void)
{
	for (BossSelectSpotBase* s : spot) { s->SelectedDraw(selectedImage[(int)Input::GetIns().LastInputKinds()]); }
}

void BossSelectSpotManager::SubRelease(void)
{
	for (BossSelectSpotBase*& s : spot) { s = nullptr; }
	DeleteGraph(selectedImage[(int)true]);
	DeleteGraph(selectedImage[(int)false]);
}