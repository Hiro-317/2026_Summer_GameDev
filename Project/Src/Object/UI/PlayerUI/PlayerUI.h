#pragma once

#include <DxLib.h>
#include <vector>

#include "../../../Common/Vector2.h"


class PlayerUI
{
private:

	static constexpr int IMAGE_SIZE = 256;

public:

	PlayerUI(Vector2I pos, const int& coolTimeCounter, int COOL_TIME);
	~PlayerUI();

	void Update(void);
	void Draw(void);

	const bool GetIsCoolTimeNow(int STATE_TAG) { return (coolTimeCounter > 0); }

private:

	// 描画位置
	Vector2I pos;

	const int& coolTimeCounter;	// クールタイムカウンター用変数
	float coolTimeRatio;	// クールタイム割合変数
	float offset;			// 描画するときのずれを修正するためのオフセット変数

	const int COOL_TIME;

	std::vector<int> images;	// 画像を読み込むためのハンドル

	//const Vector2& pos;		// 座標

};

