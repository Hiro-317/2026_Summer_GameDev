#pragma once

#include"../StageBase.h"

#include<map>

class StageManager
{
public:

	// ステージID
	enum class STAGE_ID
	{
		TOMATO_BOSS_STAGE,	// トマトボス

		MAX
	};

	StageManager();				// コンストラクタ
	~StageManager() = default;	// デストラクタ

	void Load(void);	// ロード
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void Release(void);	// 解放

	void SetStageID(STAGE_ID stageId) { stageId_ = stageId; }

private:

	std::map<STAGE_ID, StageBase*> stages_;

	STAGE_ID stageId_;

};