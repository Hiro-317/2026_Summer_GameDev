#pragma once
#include"../SceneBase.h"

#include"../../Common/Vector3.h"


class BossSelect : public SceneBase
{
public:
	BossSelect();
	~BossSelect()override = default;

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
};