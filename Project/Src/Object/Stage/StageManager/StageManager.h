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

	void ChangeStage(STAGE_ID id);		// ステージの切り替え

private:

	// ステージのインスタンス管理用変数
	std::map<STAGE_ID, StageBase*> stages;
	StageBase* currentStage;
	STAGE_ID stageId;

	int modelId[(int)STAGE_ID::MAX];
};