#pragma once

#include "../../ActorBase.h"

#include "LobbyCharaPreviewBase.h"

#include "../../CharaTypeDefine.h"

#include "../../../Manager/Net/NetWorkDefine.h"

class LobbyCharaPreviewManager : public ActorBase
{
public:
	LobbyCharaPreviewManager();
	~LobbyCharaPreviewManager()override = default;

	void Load(void)override;
	void Init(void)override {}
	void Update(void)override;
	void Draw(void)override;
	void AlphaDraw(void)override {}
	void Release(void)override;

	// SceneManagerの持つ情報をもとに指定のIDのキャラプレビューを更新する
	void ReloadChara(void);
	// SceneManagerの持つ情報をもとに指定のIDのキャラプレビューを更新する
	void ReloadChara(MSG_SENDER_ID senderId);

private:
	// 現在選択中のキャラプレビュー
	LobbyCharaPreviewBase* charaPreview[(int)MSG_SENDER_ID::Max];

	const Vector3 CHARA_PREVIEW_POS[(int)MSG_SENDER_ID::Max] = {
		Vector3::XZonly(-350.0f, -300.0f),
		Vector3::XZonly(350.0f, 300.0f),
		Vector3::XZonly(-700.0f, -600.0f),
		Vector3::XZonly(700.0f, 600.0f),
	};
};