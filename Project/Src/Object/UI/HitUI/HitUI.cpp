#include "HitUI.h"
#include "../../../pch.h"

HitUI::HitUI() :
    UI_Base()
{
}

HitUI::~HitUI()
{
}

void HitUI::Load(void)
{
}

void HitUI::SubInit(void)
{
}

void HitUI::SubUpdate(void)
{
    auto it = damageUiList.begin();
    while (it != damageUiList.end()) {
        if (!it->AliveUpdate()) { it = damageUiList.erase(it); }
        else { ++it; }
    }
}

void HitUI::SubDraw(void)
{
    for (const auto& ui : damageUiList) { ui.Draw(); }
}

void HitUI::SubRelease(void)
{
    damageUiList.clear();
}

void HitUI::DamageSetting(const short damage, const bool isCritical)
{
    // ダメージ値の設定
    AddPopup(
        std::to_string(damage),
        isCritical ? 0xffff00 : 0xffffff    // 会心ならば：黄色 / 非会心ならば：白色
    );
}

void HitUI::MissSetting()
{
    // ミス！を設定
    AddPopup(
        "\u30DF\u30B9\uFF01",   // ミス！
        0x00ffff                // 水色
    );
}


// 描画するテキストと色を設定し、描画を開始する
void HitUI::AddPopup(const std::string& text, int color)
{
    AddDamageUiInfo info;

    info.aliveTime = (short)DAMAGE_UI_ALIVE_TIME;
    info.text = text;
    info.color = color;

    info.pos.x = (float)(App::SCREEN_SIZE_X_HALF + GetRand(200) - 100);
    info.pos.y = (float)(App::SCREEN_SIZE_Y_HALF + GetRand(100) - 50);

    damageUiList.emplace_back(info);
}