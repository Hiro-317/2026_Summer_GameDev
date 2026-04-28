#pragma once

#include <DxLib.h>
#include <vector>

#include "../../../Common/Vector2.h"


class PlayerUI
{
private:

	static constexpr int IMAGE_SIZE = 256;

public:
	PlayerUI(Vector2 pos);
	~PlayerUI();

	void Update(void);
	void Draw(void);

	const bool GetIsCoolTimeNow(int STATE_TAG) { return (ui_info.coolTimeCounter > 0); }
	void SetCoolTime(int COOL_TIME) { ui_info.COOL_TIME = COOL_TIME; }
	void StartCoolTime(void) { ui_info.coolTimeCounter = ui_info.COOL_TIME; }

private:

	struct UI_INFO
	{
		// 描画位置
		Vector2 pos = Vector2(0, 0);

		float coolTimeCounter = 0.0f;	// クールタイムカウンター用変数
		float coolTimeRatio = 1.0f;	// クールタイム割合変数
		float offset = 0.0f;			// 描画するときのずれを修正するためのオフセット変数

		int COOL_TIME = 0;

		std::vector<int> images;	// 画像を読み込むためのハンドル
	};

	

	UI_INFO ui_info;
		
};

