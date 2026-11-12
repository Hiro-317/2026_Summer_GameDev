#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

#include <algorithm>

#include "../../../../Common/Vector2I.h"

class PlayerStaminaUI : public ActorBase
{
private:

	static const int IMAGE_MAX = 7;
	const int IMAGE_X_NUM = 7;
	const int IMAGE_Y_NUM = 1;

	const int IMAGE_X_SIZE = 50;
	const int IMAGE_Y_SIZE = 50;

public:

	/// <summary>
	/// スタミナのUIのコンストラクタ
	/// </summary>
	/// <param name="STAMINA_MAX">スタミナの最大値</param>
	/// <param name="stamina">スタミナの参照</param>
	PlayerStaminaUI(short STAMINA_MAX, const float& stamina, const Vector3& playerPos);
	// デストラクタ
	~PlayerStaminaUI()override = default;

	// ロード
	void Load(void) override;
	// 更新
	void Update(void)override {}

private:

	// スタミナの割合で配列を動かす
	bool GetStaminaIndex(float stamina, short STAMINA_MAX, int& index) {

		float rate = (float)stamina / (float)STAMINA_MAX;

		if (rate == 1.0f) { return false; }

		index = (int)(rate * IMAGE_MAX + 0.5);

		index = std::clamp(index, 0, IMAGE_MAX - 1);

		return true;
	}

	// 画像ハンドル
	int images[IMAGE_MAX];

	// スタミナの最大値
	const short STAMINA_MAX;

	// スタミナの参照
	const float& stamina;

	// プレイヤー座標
	const Vector3& playerPos;

	// 初期化
	void SubInit(void)override;
	// 描画
	void SubDraw(void)override;
	// 解放
	void SubRelease(void)override;
};