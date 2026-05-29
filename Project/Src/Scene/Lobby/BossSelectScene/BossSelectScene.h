#pragma once
#include "../../SceneBase.h"

class BossSelectScene : public SceneBase
{
public:
	BossSelectScene();
	~BossSelectScene();

void Load(void) override;
void Init(void) override;
void Update(void) override;
void Draw(void) override;
void Release(void) override;

private:
	int image;
};
