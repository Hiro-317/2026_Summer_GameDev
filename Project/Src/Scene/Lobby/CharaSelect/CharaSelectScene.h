#pragma once

#include <functional>

#include "../../SceneBase.h"

#include "../../../Object/CharaTypeDefine.h"

class CharaSelectScene : public SceneBase
{
public:
	CharaSelectScene(const std::function<void(void)>& LobbyPreviewCharaChange);
	~CharaSelectScene()override = default;

	// 読み込み
	void Load(void)override;
	// 更新ステップ
	void Update(void)override;
	// 描画処理
	void Draw(void)override;

private:
	
	const std::function<void(void)> LobbyPreviewCharaChange;

	// 選択中のキャラタイプ
	CHARA_TYPE selectCharaType;
};