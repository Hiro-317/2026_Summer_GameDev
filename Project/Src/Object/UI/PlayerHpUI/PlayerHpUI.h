#pragma once
#include <vector>

#include "../../../Common/Vector2.h"

class PlayerHpUI
{
public:
	PlayerHpUI(
		const short& player1hp,	const short PLAYER1_HP_MAX,
		const short& player2hp,	const short PLAYER2_HP_MAX,
		const short& player3hp,	const short PLAYER3_HP_MAX,
		const short& player4hp,	const short PLAYER4_HP_MAX
		);

	~PlayerHpUI();

	void Update(void);	// 更新
	void Draw(void);	// 描画
	void Release(void);	// 解放

private:

	// HP画像の枚数
	const int HP_IMAGE_MAX = 1;

	// HPの最大値
	const int PLAYER1_HP_MAX;
	const int PLAYER2_HP_MAX;
	const int PLAYER3_HP_MAX;
	const int PLAYER4_HP_MAX;

	// プレイヤーのHP
	const int& player1hp;
	const int& player2hp;
	const int& player3hp;
	const int& player4hp;

	// HPの画像
	std::vector<int> hpImages;

	// HPの枠
	int hpFrameImage;
};	