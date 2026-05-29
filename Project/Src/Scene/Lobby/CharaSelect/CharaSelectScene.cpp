#include "CharaSelectScene.h"


CharaSelectScene::CharaSelectScene(const std::function<void(void)>& LobbyPreviewCharaChange) :
	LobbyPreviewCharaChange(LobbyPreviewCharaChange),

	frameImage(-1)
{
}

void CharaSelectScene::Load(void)
{

}

void CharaSelectScene::Init(void)
{

}

void CharaSelectScene::Update(void)
{

}

void CharaSelectScene::Draw(void)
{

}

void CharaSelectScene::Release(void)
{
	LobbyPreviewCharaChange();
}
