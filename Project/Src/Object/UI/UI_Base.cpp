#include "UI_Base.h"

UI_Base::UI_Base()
{
}

void UI_Base::Init(void)
{
	SubInit();
}

void UI_Base::Update(void)
{
	SubUpdate();
}

void UI_Base::Draw(void)
{
	SubDraw();
}

void UI_Base::Release(void)
{
	SubRelease();

	// 画像の解放
	for (const auto& image : uiImages)
	{
		if (image.second == -1) continue;

		DeleteGraph(image.second);
	}
	uiImages.clear();
}