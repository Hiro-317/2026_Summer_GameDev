#pragma once

#include"../ActorBase.h"

#include"../../Common/Vector3.h"
#include"../../Utility/Utility.h"

#include<string>

class StageBase : public ActorBase
{
public:

#pragma region 定数定義
	static constexpr float MODEL_SCALE = 1.0f;	// モデルのスケール
#pragma endregion


	StageBase(const std::string& path);				// コンストラクタ
	virtual ~StageBase() = default;		// デストラクタ

	virtual void Load(void) override;		// ステージの情報を読み込む
	virtual void Init(void)override;		// 初期化処理
	virtual void Update(void)override = 0;		// 更新処理
	virtual void Draw(void)override = 0;		// 描画処理
	virtual void Release(void)override = 0;		// 解放処理


};