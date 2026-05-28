#include "DamageUI.h"
#include "../../../pch.h"

DamageUI::DamageUI() :
    UI_Base()
{
}

DamageUI::~DamageUI()
{
}

void DamageUI::Load(void)
{
}

void DamageUI::SubInit(void)
{
}

void DamageUI::SubUpdate(void)
{
    auto it = damageUiList.begin();

    while (it != damageUiList.end()) {
        if (!it->AliveUpdate()) { it = damageUiList.erase(it); }
        else { ++it; }
    }
}

void DamageUI::SubDraw(void)
{
    for (const auto& ui : damageUiList) { ui.Draw(); }
}

void DamageUI::SubRelease(void)
{
    damageUiList.clear();
}

void DamageUI::DamageSetting(short damage, bool isCritical)
{
    AddDamageUiInfo info;

    info.aliveTime = (short)DAMAGE_UI_ALIVE_TIME;
    info.damageValue = damage;
    info.isCritical = isCritical;

    // ランダム出現位置
    info.pos.x =
        (float)(App::SCREEN_SIZE_X_HALF
            + GetRand(200) - 100);
    info.pos.y =
        (float)(App::SCREEN_SIZE_Y_HALF
            + GetRand(100) - 50);

    // 設定した値をリストに登録する
    damageUiList.emplace_back(info);
}