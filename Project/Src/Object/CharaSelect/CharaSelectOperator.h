#pragma once

#include "../../Manager/Net/NetWorkDefine.h"

#include "../ActorBase.h"
#include "CharaSelectPreview/CharaSelectPreviewBase.h"

#include "../CharaTypeDefine.h"

class CharaSelectOperator : public ActorBase
{
public:
	CharaSelectOperator(MSG_SENDER_ID operatorSenderId);
	~CharaSelectOperator()override = default;

	void Load(void)override;
	void Init(void)override;
	void ReceptionUpdate(void)override;
	void Update(void)override;
	void SendUpdate(void)override;
	void Draw(void)override;
	void AlphaDraw(void)override;
	void UiDraw(void)override;
	void Release(void)override;

private:
	// 自身の操作権を持つ送信者ID
	const MSG_SENDER_ID operatorSenderId;

	// 選択中キャラのプレビューオブジェクト配列
	CharaSelectPreviewBase* charaPreview[(int)CHARA_TYPE::Max];
	const Vector3 CHARA_SELECT_PREVIEW_POS[(int)MSG_SENDER_ID::Max] =
	{
		Vector3::Xonly(-225.0f),
		Vector3::Xonly(-75.0f),
		Vector3::Xonly(75.0f),
		Vector3::Xonly(225.0f),
	};

	// 選択中のキャラタイプ
	CHARA_TYPE selectCharaType;
};