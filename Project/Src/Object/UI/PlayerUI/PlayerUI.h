#pragma once

#include <DxLib.h>
#include <vector>

#include "../../../Common/Vector2.h"


class PlayerUI
{
private:

	static constexpr int IMAGE_SIZE = 256;
	const int COOL_TIME;

public:
	PlayerUI(Vector2 pos, int COOL_TIME);
	~PlayerUI();

	void Update(void);
	void Draw(void);

	const bool GetIsCoolTimeNow(void) { return (coolTimeCounter > 0); }

private:

	// 描画位置
	Vector2 pos;

	float coolTimeCounter;	// クールタイムカウンター用変数
	float coolTimeRatio;	// クールタイム割合変数
	float offset;			// 描画するときのずれを修正するためのオフセット変数
		
	std::vector<int> images;	// 画像を読み込むためのハンドル
};

