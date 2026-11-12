#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

#include <array>

#include "../../../../Common/Vector2I.h"

#include "../../../../Manager/Net/SenderID_Define.h"

class ToPlayerTargetUI : public ActorBase
{
public:
	ToPlayerTargetUI(
		const std::array<Vector2I,(size_t)MSG_SENDER_ID::Max>& DRAW_POS_TABLE,
		const unsigned char& targetPlayerIndex
	);

	~ToPlayerTargetUI()override = default;

	void Load(void) override;
private:

	// 描画位置テーブル
	const std::array<Vector2I, (size_t)MSG_SENDER_ID::Max>& DRAW_POS_TABLE;

	// 選択番号
	const unsigned char& targetPlayerIndex;

	// 画像ハンドル（選択中フレーム）
	int selectedFrameImage;

	// 初期化処理
	void SubInit(void)override;

	// 描画処理
	void SubDraw(void)override;

	// 解放処理
	void SubRelease(void)override;
};