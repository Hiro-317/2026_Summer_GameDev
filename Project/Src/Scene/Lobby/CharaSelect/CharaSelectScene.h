#pragma once

#include <functional>

#include "../../SceneBase.h"

#include "../../../Object/CharaTypeDefine.h"

class CharaSelectScene : public SceneBase
{
public:
	CharaSelectScene(
		const std::function<void(void)>& LobbyPreviewCharaChange,
		const std::function<void(void)>& LobbyReceptionUpdate = nullptr
	);
	~CharaSelectScene()override = default;

	// 読み込み
	void Load(void)override;
	// 更新ステップ
	void Update(void)override;
	// 描画処理
	void Draw(void)override;

private:
	
	// ロビー画面のキャラプレビューを更新する関数のポインターを受け取る
	const std::function<void(void)> LobbyPreviewCharaChange;

	// マルチロビー画面の受信処理を受け取る
	const std::function<void(void)> LobbyReceptionUpdate;

	// 選択中のキャラタイプ
	CHARA_TYPE selectCharaType;
};