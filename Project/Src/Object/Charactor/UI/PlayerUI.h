#pragma once

#include <DxLib.h>
#include <vector>

class PlayerUI
{
private:
	static constexpr int SIZE = 256;
public:

	PlayerUI();
	~PlayerUI();

	void Draw(void);

private:
	std::vector<int> images;
};

