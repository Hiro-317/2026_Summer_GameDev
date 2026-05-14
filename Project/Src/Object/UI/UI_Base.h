#pragma once

#include <DxLib.h>
#include <vector>
#include <map>
#include <string>

class UI_Base
{
public:

	UI_Base();
	virtual ~UI_Base() = default;

	virtual void Load(void) = 0;
	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

protected:

	enum class FILE_PATH_TYPE
	{
		HP,
		STAMINA,
		SKILL,

		MAX
	};

	std::map<int, int> uiImages;	// UIの画像

	std::vector<std::string> uiKindsPath = {
		"Data/Image/UI/Player/HP/",
		"Data/Image/UI/Player/Stamina/",
		"Data/Image/UI/Player/Skill/"
	};

	void LoadUIImage(const std::string& path, int kinds,  FILE_PATH_TYPE pathType)
	{
		int pathIndex = (int)pathType;

		// 範囲チェック
		if (pathIndex < 0 || pathIndex >= (int)uiKindsPath.size()) { return; }

		// フルパス作成
		std::string fullPath = uiKindsPath[pathIndex] + path + ".png";

		// 画像ロードして保存
		uiImages[kinds] = LoadGraph(fullPath.c_str());
	}

	virtual void SubInit(void) {}
	virtual void SubUpdate(void) {}
	virtual void SubDraw(void) {}
	virtual void SubRelease(void) {}
};
