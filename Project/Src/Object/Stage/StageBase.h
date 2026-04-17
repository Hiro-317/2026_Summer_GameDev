#pragma once

#include"../ActorBase.h"

#include"../../Common/Vector3.h"
#include"../../Utility/Utility.h"

#include<string>

class StageBase : public ActorBase
{
public:
	StageBase(int modelId);				// コンストラクタ
	virtual ~StageBase() = default;		// デストラクタ

	virtual void Load(void) = 0;		// ステージの情報を読み込む
	virtual void Init(void) = 0;		// 初期化処理
	virtual void Update(void) = 0;		// 更新処理
	virtual void Draw(void) = 0;		// 描画処理
	virtual void Release(void) = 0;		// 解放処理

protected:

	struct StageInfo
	{
		int modelId_ = -1;	// モデルのハンドル

		// 座標、回転、拡大
		Vector3 pos_ = Vector3(0,0,0);
		Vector3 rot_ = Vector3(0,0,0);
		Vector3 scale_ = Vector3(1,1,1);
	};

	// ステージの情報
	StageInfo stageInfo_;

};