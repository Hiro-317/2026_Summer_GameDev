#include"StageBase.h"

StageBase::StageBase(const std::string& path) : ActorBase()
{
	trans.Load(path);
}

void StageBase::Load(void)
{
}

void StageBase::Init(void)
{
	// ステージの初期設定
	SetDynamicFlg(false);
	SetGravityFlg(false);
	SetPushFlg(true);
	SetPushWeight(100);
	SetGravityFlg(false);

	trans.scale = MODEL_SCALE;
}
