#pragma once

#include "../../pch.h"
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

	std::vector<int> uiImages;	// UIの画像

	std::vector<std::string> uiKindsPath = {
		"Data/Image/UI/Player/HP/",
		"Data/Image/UI/Player/Stamina/",
		"Data/Image/UI/Player/Skill/"
	};

	/// <summary>
	/// 指定された画像を指定された配列の変数にロードする
	/// </summary>
	/// <param name="path">画像のパス(".png"は除く)</param>
	/// <param name="kinds">画像をロードする配列を指定</param>
	/// <param name="pathType">HP or スタミナ or スキル</param>
	void UILoadImage(const std::string& path, int kinds,  FILE_PATH_TYPE pathType)
	{
		int pathIndex = (int)pathType;
		if (pathIndex < 0 || pathIndex >= (int)uiKindsPath.size()) { return; }

		// パス作成
		std::string fullPath = uiKindsPath[pathIndex] + path + ".png";

		// 必要サイズ確保
		if (kinds >= static_cast<int>(uiImages.size()))	{
			// 指定された画像をロード
			uiImages.resize(kinds + 1, LoadGraph(fullPath.c_str()));
		}
	}

	virtual void SubInit(void) {}
	virtual void SubUpdate(void) {}
	virtual void SubDraw(void) {}
	virtual void SubRelease(void) {}
};
