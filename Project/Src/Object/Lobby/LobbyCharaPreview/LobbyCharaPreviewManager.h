#pragma once

#include "../../ActorBase.h"

#include "LobbyCharaPreviewBase.h"

#include "../../CharaTypeDefine.h"

class LobbyCharaPreviewManager : public ActorBase
{
public:
	LobbyCharaPreviewManager();
	~LobbyCharaPreviewManager()override = default;

	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void AlphaDraw(void)override {}
	void Release(void)override;

	// キャラを変更
	void ChangeChara(CHARA_TYPE type);

private:
	// 現在選択中のキャラプレビュー
	LobbyCharaPreviewBase* charaPreview;
};