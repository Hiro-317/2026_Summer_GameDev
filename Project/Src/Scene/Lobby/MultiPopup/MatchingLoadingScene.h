#pragma once

#include "../../SceneBase.h"

class MatchingLoadingScene :public SceneBase
{
public:
	MatchingLoadingScene();
	~MatchingLoadingScene()override = default;

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

	// マッチングを待つ時間
	const short MATCHING_WAIT_TIME = 1000;

	// マッチング待ち計測用カウンター
	short waitCounter;
};