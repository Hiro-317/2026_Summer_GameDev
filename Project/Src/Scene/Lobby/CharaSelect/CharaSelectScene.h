#pragma once

#include <functional>

#include "../../SceneBase.h"

class CharaSelectScene : public SceneBase
{
public:
	CharaSelectScene(const std::function<void(void)>& LobbyPreviewCharaChange);
	~CharaSelectScene()override = default;

	// 読み込み
	void Load(void)override;
	// 初期化処理
	void Init(void)override;
	// 更新ステップ
	void Update(void)override;
	// 描画処理
	void Draw(void)override;
	// 解放処理
	void Release(void)override;

private:

	// 枠の画像
	int frameImage;
	
	const std::function<void(void)> LobbyPreviewCharaChange;

};