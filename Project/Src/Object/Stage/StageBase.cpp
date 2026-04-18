#include"StageBase.h"

StageBase::StageBase(int model) : ActorBase()
{
	trans.model = model;
}

void StageBase::Init(void)
{
	// ステージの初期設定
	SetDynamicFlg(false);
	SetGravityFlg(false);
	SetPushFlg(true);
	SetPushWeight(100);
}
