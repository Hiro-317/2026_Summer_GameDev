#pragma once

#include"../SceneBase.h"

class GameOver : public SceneBase
{
public:
	GameOver();
	~GameOver()override;

	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:

	int image;
};
