#include "TomatoSelect.h"


TomatoSelect::TomatoSelect()
{
}

void TomatoSelect::Load(void)
{
	// モデルを読み込む
	trans.Load("Charactor/Orange/OrangePlayer");
	
	// モデルのスケールを設定
	trans.scale = MODEL_SCALE;

	// モデルの中心点のズレ
	trans.centerDiff = MODEL_CENTER_DIFF;

	CharaSelectObj::Load();
}
