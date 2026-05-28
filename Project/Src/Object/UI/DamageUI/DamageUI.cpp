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

    while (it != damageUiList.end())
    {
        if (!it->AliveUpdate())
        {
            it = damageUiList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void DamageUI::SubDraw(void)
{
    for (const auto& ui : damageUiList)
    {
        ui.Draw();
    }
}

void DamageUI::SubRelease(void)
{
    damageUiList.clear();
}

short DamageUI::DamageSetting(const short damage)
{
    AddDamageUiInfo info;

    info.aliveTime = DAMAGE_UI_ALIVE_TIME;
    info.damageValue = damage;

    damageUiList.emplace_back(info);

    return damage;
}