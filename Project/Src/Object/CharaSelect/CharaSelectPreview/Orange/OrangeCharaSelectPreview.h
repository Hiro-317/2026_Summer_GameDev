#pragma once

#include "../CharaSelectPreviewBase.h"

class OrangeCharaSelectPreview : public CharaSelectPreviewBase
{
public:
	OrangeCharaSelectPreview(MSG_SENDER_ID operatorSenderId, const Vector3& pos) :
		CharaSelectPreviewBase(operatorSenderId, pos)
	{
	}
	~OrangeCharaSelectPreview()override = default;

	void Load(void)override {
		// モデルを読み込む
		trans.Load("Charactor/Orange/OrangePlayer");
		// アニメーションを登録する
		AddAnimation("Data/Model/Charactor/CommonAnime/Dance.mv1", 1.0f);
	}
};