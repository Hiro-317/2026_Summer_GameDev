#pragma once

#include"../ActorBase.h"

#include"../../Common/Vector3.h"
#include"../../Utility/Utility.h"

#include<string>

class StageBase : public ActorBase
{
public:

	StageBase(int model);				// コンストラクタ
	virtual ~StageBase() = default;		// デストラクタ

	virtual void Load(void) = 0;		// ステージの情報を読み込む
	virtual void Init(void);		// 初期化処理
	virtual void Update(void) = 0;		// 更新処理
	virtual void Draw(void) = 0;		// 描画処理
	virtual void Release(void) = 0;		// 解放処理


};