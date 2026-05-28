#include "LobbyCharaPreviewBase.h"

#include "../../Common/AnimationController/AnimationController.h"

LobbyCharaPreviewBase::LobbyCharaPreviewBase(const Vector3& pos) :
	ActorBase(),

	anime(nullptr),
	playAnimeType(0)
{
	trans.pos = pos;
}

void LobbyCharaPreviewBase::AnimationControllerCreate(void)
{
	// 生成していなければアニメーションコントローラーを生成
	if (anime == nullptr) { anime = new AnimationController(trans.model); }
}

void LobbyCharaPreviewBase::SubInit(void)
{
#pragma region 基底クラスにある機能の挙動設定
	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(false);
	// 衝突時の押し出しを無効にする
	SetPushFlg(false);
#pragma endregion

	// アニメーション再生
	if (anime) { anime->Play(playAnimeType); }
}

void LobbyCharaPreviewBase::Update(void)
{
	// アニメーション更新
	if (anime) { anime->Update(); }
}

void LobbyCharaPreviewBase::SubRelease(void)
{
	// アニメーションコントローラーを削除
	if (anime) {
		anime->Release();
		delete anime;
		anime = nullptr;
	}
}

void LobbyCharaPreviewBase::AddInFbxAnimation(int inFbxMaxIndex, float speed, int playAnimeType)
{
	AnimationControllerCreate();
	for (int index = 0; index < inFbxMaxIndex; index++) {
		anime->AddInFbx(index, speed, index);
	}
	this->playAnimeType = playAnimeType;
}

void LobbyCharaPreviewBase::AddAnimation(const char* filePath, float speed, int index)
{
	AnimationControllerCreate();
	anime->Add(index, speed, filePath);
	playAnimeType = index;
}