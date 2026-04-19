#include "StageManager.h"

#include"../TomatoBoss/TomatoBossStage.h" 

StageManager::StageManager() :
    currentStage(nullptr),
    stageId(STAGE_ID::TOMATO_BOSS_STAGE),
    stages()
{
}

void StageManager::Load(void)
{
	// ステージの登録・生成
	stages.emplace(STAGE_ID::TOMATO_BOSS_STAGE, new TomatoBossStage("Stage/TomatoBoss/TomatBossStage"));
}

void StageManager::Init(void)
{
    // 最初に設定されるステージを指定
    ChangeStage(STAGE_ID::TOMATO_BOSS_STAGE);
}

void StageManager::Release(void)
{
    // マップの解放
	stages.clear();
}

void StageManager::ChangeStage(STAGE_ID id)
{
    if(currentStage)
    {
        // 現在のステージを解放
        currentStage->Release();
    }

    auto it = stages.find(id);
	if (it != stages.end())
    {
        // ステージを切り替える
        currentStage = it->second;
        stageId = id;

        // 新しく切り替えたステージを初期化
        currentStage->Init();
    }

}
