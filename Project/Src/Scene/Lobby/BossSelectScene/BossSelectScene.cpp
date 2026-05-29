#include "BossSelectScene.h"
#include"../../../Manager/Font/FontManager.h"
#include "../../../Application/Application.h"

BossSelectScene::BossSelectScene()
{
}

BossSelectScene::~BossSelectScene()
{
}

void BossSelectScene::Load(void)
{
	image = LoadGraph("Data/Image/TOMATO.png");
}

void BossSelectScene::Init(void)
{
}

void BossSelectScene::Update(void)
{
}

void BossSelectScene::Draw(void)
{
    DrawFormatStringToHandle(0, 0, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_64), "ボス選択");
    int i = DrawGraph(0, 0, image, false);
	if (i == -1) {
		int aa = 0;
		aa++;
	}

    // デバッグチェック
    if (image < 0) {
        // ログ出力やデバッグ表示を追加
        printf("Draw: image handle invalid: %d\n", image);
        return;
    }

    int w = 0, h = 0;
    if (GetGraphSize(image, &w, &h) != 0) {
        printf("GetGraphSize failed for handle %d\n", image);
    }
    else {
        printf("Handle %d size: %dx%d\n", image, w, h);
    }
}

void BossSelectScene::Release(void)
{
	DeleteGraph(image);
}
