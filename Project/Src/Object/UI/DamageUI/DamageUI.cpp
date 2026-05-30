#include "DamageUI.h"
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

/// <summary>
/// ダメージ値の設定
/// </summary>
/// <param name="damage">表示するダメージ値</param>
/// <param name="isCritical">会心ならば true / 非会心ならば false</param>
void HitUI::DamageSetting(const short damage, const bool isCritical)
{
    AddPopup(
        std::to_string(damage),
        isCritical ? 0xffff00 : 0xffffff
    );
}

// ミス！を表示する関数
void HitUI::MissSetting()
{
    AddPopup(
        "\u30DF\u30B9\uFF01",
        0x00ffff
    );
}

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