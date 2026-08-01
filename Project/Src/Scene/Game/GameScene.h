#pragma once

#include"../SceneBase.h"

class GameScene : public SceneBase
{
public:
	GameScene();
	~GameScene()override = default;

private:

	// 読み込み
	void SubPostLoad(void)override;
	// 初期化（メイン処理の後）
	void SubPostInit(void)override;
	// 更新（Actor更新・当たり判定の後、Camera更新の前）
	void SubPostUpdate(void)override;

	// カメラ状態変更フラグ
	bool focusFlg;
};