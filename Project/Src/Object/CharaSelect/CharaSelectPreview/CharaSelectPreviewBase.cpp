#include "CharaSelectPreviewBase.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../Common/AnimationController/AnimationController.h"

CharaSelectPreviewBase::CharaSelectPreviewBase(MSG_SENDER_ID operatorSenderId, const Vector3& pos) :
	ActorBase(),

	operatorSenderId(operatorSenderId),

	anime(nullptr),
	playAnimeType(0)
{
	trans.pos = pos;
}

void CharaSelectPreviewBase::AnimationControllerCreate(void)
{
	// 生成していなければアニメーションコントローラーを生成
	if (anime == nullptr) { anime = new AnimationController(trans.model); }
}

void CharaSelectPreviewBase::SubInit(void)
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

void CharaSelectPreviewBase::Select(void)
{
	// アニメーションを最初から再生しなおす
	anime->Stop();
	anime->Play(playAnimeType);
}

void CharaSelectPreviewBase::ReceptionUpdate(void)
{
	if (anime) {
		while (MsgDataPlayerAnimeStep* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerAnimeStep>(operatorSenderId)) {
			// 自分のキャラ（操作対象）の場合
			if (Net::GetIns().GetSenderId() == operatorSenderId) {
				// ホストから送られた座標と今の自分の座標の距離を測る
				float diff = anime->GetStep() - dataPtr->animeStep;
				// 誤差が小さいなら無視
				if (diff > 0.5f) {
					// 誤差が大きい場合、少しずつホストから送られた数値に寄せる（補間）
					anime->SetStep(anime->GetStep() * 0.9f + dataPtr->animeStep * 0.1f);
				}
			}
			// 他人のキャラなら、そのまま同期
			else { anime->SetStep(dataPtr->animeStep); }

			delete dataPtr;
		}
	}
}

void CharaSelectPreviewBase::Update(void)
{
	// アニメーション更新
	if (anime) { anime->Update(); }
}

void CharaSelectPreviewBase::SendUpdate(void)
{
	// アニメーション再生ステップを送信
	if (anime) {
		Net::GetIns().Send(MsgDataPlayerAnimeStep(anime->GetStep()), operatorSenderId);
	}
}

void CharaSelectPreviewBase::UiDraw(void)
{

}

void CharaSelectPreviewBase::SubRelease(void)
{
	// アニメーションコントローラーを削除
	if (anime) {
		anime->Release();
		delete anime;
		anime = nullptr;
	}
}

void CharaSelectPreviewBase::AddInFbxAnimation(int inFbxMaxIndex, float speed, int playAnimeType)
{
	AnimationControllerCreate();
	for (int index = 0; index < inFbxMaxIndex; index++) {
		anime->AddInFbx(index, speed, index);
	}
	this->playAnimeType = playAnimeType;
}

void CharaSelectPreviewBase::AddAnimation(const char* filePath, float speed, int index)
{
	AnimationControllerCreate();
	anime->Add(index, speed, filePath);
	playAnimeType = index;
}