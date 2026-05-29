#pragma once

#include "../../Manager/Net/NetWorkDefine.h"

#include "../ActorBase.h"
#include "CharaSelectPreview/CharaSelectPreviewBase.h"

#include "../CharaTypeDefine.h"

class CharaSelectPreviewManager : public ActorBase
{
public:
	CharaSelectPreviewManager();
	~CharaSelectPreviewManager()override = default;

	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void AlphaDraw(void)override {}
	void Release(void)override;

	const CHARA_TYPE GetCharaType(void) { return selectCharaType; }
private:

	// 枠の画像
	int frameImage;

	// 選択中キャラのプレビューオブジェクト配列
	CharaSelectPreviewBase* charaPreview[(int)CHARA_TYPE::Max];

	// 選択中のキャラタイプ
	CHARA_TYPE selectCharaType;
};