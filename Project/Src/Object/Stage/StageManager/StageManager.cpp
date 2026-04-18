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
	stages.emplace(STAGE_ID::TOMATO_BOSS_STAGE, new TomatoBossStage(-1));

    

}

void StageManager::Init(void)
{
    ChangeStage(STAGE_ID::TOMATO_BOSS_STAGE);
}

void StageManager::Update(void)
{
    if (currentStage)
    {
        currentStage->Update();
    }
}

void StageManager::Draw(void)
{
    if (currentStage)
    {
        currentStage->Draw();
    }
}

void StageManager::Release(void)
{
    for (auto& stage : stages)
    {
        if (stage.second)
        {
            stage.second->Release();
            delete stage.second;
            stage.second = nullptr;
        }
    }
	stages.clear();
}

void StageManager::ChangeStage(STAGE_ID id)
{
    if(currentStage)
    {
        currentStage->Release();
    }

    auto it = stages.find(id);
	if (it != stages.end())
    {
        currentStage = it->second;
        stageId = id;

        currentStage->Init();
    }

}
