#pragma once

#include "../SceneBase.h"

class BossSelectScene : public SceneBase
{
public:
	BossSelectScene();
	~BossSelectScene()override = default;

private:

#pragma region å—vŠÖ”Ä’è‹`

	// “Ç‚İ‚İ
	void SubPostLoad(void)override;

	// ‰Šú‰»
	void SubPostInit(void)override;

#pragma endregion

	void CreateCamera(void)override;
};