#pragma once

#include "../../Common/ActorBase/ActorBase.h"

class BossSelectSpotBase;

class BossSelectSpotManager : public ActorBase
{
public:
	BossSelectSpotManager();
	~BossSelectSpotManager()override = default;

	void Load(void)override;

private:

	// 各スポットの実体
	std::vector<BossSelectSpotBase*> spot;

	// 飛行船がスポットに近づいたときに表示する共通UI
	int selectedImage[2];

	void SubDraw(void)override;
	void SubRelease(void)override;

};