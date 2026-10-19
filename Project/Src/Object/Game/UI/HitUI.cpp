#include "HitUI.h"

#include "../../../pch.h"

#include "../../../Manager/TimeScale/TimeScale.h"

HitUI::HitUI()
{
}

void HitUI::Load(void)
{
}

void HitUI::Init(void)
{
    // UI描画に設定する
    SetDrawType(ACTOR_DRAW_TYPE::Ui);
}

void HitUI::Update(void)
{
    auto it = hitUiList.begin();
    while (it != hitUiList.end()) {
        if (!it->AliveUpdate()) { it = hitUiList.erase(it); }
        else { ++it; }
    }
}

void HitUI::SubDraw(void)
{
    for (const AddHitUiInfo& ui : hitUiList) { ui.Draw(); }
}

void HitUI::SubRelease(void)
{
    hitUiList.clear();
}

void HitUI::DamageSetting(const Vector3& collisionPos, const short damage, const bool isCritical)
{
    Vector3 temp = ConvWorldPosToScreenPos(collisionPos.ToVECTOR());
    Vector2 drawPos = Vector2(temp.x, temp.y);

    // ダメージ値の設定
    AddPopup(
        drawPos,
        std::to_string(damage),
        isCritical ? 0xffff00 : 0xffffff    // 会心ならば：黄色 / 非会心ならば：白色
    );
}

void HitUI::MissSetting(const Vector3& collisionPos)
{
    Vector3 temp = ConvWorldPosToScreenPos(collisionPos.ToVECTOR());
    Vector2 drawPos = Vector2(temp.x, temp.y);

    // ミス！を設定
    AddPopup(
        drawPos,
        "\u30DF\u30B9\uFF01",   // ミス！
        0x00ffff                // 水色
    );
}

// 描画するテキストと色を設定し、描画を開始する
void HitUI::AddPopup(const Vector2 pos, const std::string& text, int color)
{
    AddHitUiInfo info;

    info.aliveTime = HIT_UI_ALIVE_TIME;
    info.text = text;
    info.color = color;

    info.pos = pos;

    hitUiList.emplace_back(info);
}

bool HitUI::AddHitUiInfo::AliveUpdate(void)
{
    // 生存時間更新
    aliveTime -= TimeScale::Get();

    // 生存時間が切れたら「false」を返す
    if (aliveTime <= 0.0f) { return false; }

    // まだ残っていたら「true」を返す
    return true;
}

void HitUI::AddHitUiInfo::Draw(void)const
{
    float elapsedTime = HIT_UI_ALIVE_TIME - aliveTime;
    float drawPosY;

    if (elapsedTime < DAMAGE_UI_BOUNCE_START_TIME) {
        // 上昇    
        drawPosY = pos.y + DAMAGE_UI_BOUNCE_TIME - (elapsedTime * 3.0f);
    }
    else {
        float bounceTime = elapsedTime - DAMAGE_UI_BOUNCE_START_TIME;
        float bounceRate = 1.0f - (bounceTime / DAMAGE_UI_BOUNCE_TIME);
        UtilityClamp(bounceRate, 0.0f, 1.0f);

        // バウンド
        drawPosY = pos.y - (DAMAGE_UI_BOUNCE_START_TIME + fabsf(sinf(bounceTime * 0.35f)) * DAMAGE_UI_BOUNCE_POWER * bounceRate);
    }

    int alpha = 255;

    // バウンドが終了したら徐々に消える
    if (elapsedTime >= DAMAGE_UI_BOUNCE_START_TIME) {
        float fadeRate = 1.0f - ((elapsedTime - DAMAGE_UI_BOUNCE_START_TIME) / DAMAGE_UI_BOUNCE_TIME);
        UtilityClamp(fadeRate, 0.0f, 1.0f);
        fadeRate *= fadeRate;
        alpha = (int)(255 * fadeRate);
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    // 文字の表示
    DrawFormatStringToHandle(
        (int)pos.x,
        (int)drawPosY,
        color,
        Font::GetIns().GetFont(FontKinds::HIT_UI_64),
        text.c_str()
    );

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}