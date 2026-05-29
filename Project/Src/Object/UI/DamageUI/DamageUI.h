#pragma once
#include "../UI_Base.h"
#include "../../../Application/Application.h"

#include <list>
#include <algorithm>

#include "../../../Common/Vector2.h"
#include "../../../Manager/Font/FontManager.h"

#include "../../../Utility/Utility.h"

class DamageUI : public UI_Base
{
public:

    DamageUI();
    ~DamageUI() override;

    void Load(void) override;
    void SubInit(void) override;
    void SubUpdate(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;

    void DamageSetting(short damage, bool isCritical = false);

private:

    static constexpr float DAMAGE_UI_ALIVE_TIME = 120.0f;
    static constexpr float DAMAGE_UI_BOUNCE_START_TIME = DAMAGE_UI_ALIVE_TIME / 3;
    static constexpr float DAMAGE_UI_BOUNCE_TIME = DAMAGE_UI_ALIVE_TIME - DAMAGE_UI_BOUNCE_START_TIME;
    static constexpr float DAMAGE_UI_BOUNCE_POWER = 12.0f;

    struct AddDamageUiInfo
    {
        // 生存時間更新
        bool AliveUpdate(int value = 1) {
            if (aliveTime < value) { return false; }
            aliveTime -= value;
            if (aliveTime <= 0) { return false; }
            return true;
        }

        // 描画
        void Draw(void) const
        {
            float elapsedTime = (float)(DAMAGE_UI_ALIVE_TIME - aliveTime);
            float drawPosY;

            if (elapsedTime < DAMAGE_UI_BOUNCE_START_TIME) {
                // ダメージ表記を徐々に上にあげる
                drawPosY = pos.y + DAMAGE_UI_BOUNCE_TIME - (elapsedTime * 3.0f);
            }
            else {
                float bounceTime = elapsedTime - DAMAGE_UI_BOUNCE_START_TIME;
                float bounceRate = 1.0f - (bounceTime / DAMAGE_UI_BOUNCE_TIME);
                UtilityClamp(bounceRate, 0.0f, 1.0f);

                // バウンド開始
                drawPosY = pos.y - (DAMAGE_UI_BOUNCE_START_TIME + fabs(sinf(bounceTime * 0.35f)) * DAMAGE_UI_BOUNCE_POWER * bounceRate);
            }

            int alpha = 255;

            // ダメージ表記がバウンドし始めたら徐々に消え始める
            if (elapsedTime >= DAMAGE_UI_BOUNCE_START_TIME) {
                float fadeRate = 1.0f - ((elapsedTime - DAMAGE_UI_BOUNCE_START_TIME) / DAMAGE_UI_BOUNCE_TIME);
                UtilityClamp(fadeRate, 0.0f, 1.0f);
                fadeRate *= fadeRate;
                alpha = (int)(255 * fadeRate);
            }

            // ダメージの描画
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
            DrawFormatStringToHandle(
                (int)pos.x,
                (int)drawPosY,
                (isCritical) ? criticalColor : normalColor,
                Font::GetIns().GetFont(FontKinds::DEFAULT_64),
                "%d",
                damageValue
            );
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        const int normalColor = 0xffffff;
        const int criticalColor = 0xffff00;

        bool isCritical = false;    // 会心が出ているかどうか
        short aliveTime = 0;        // 表示時間管理変数
        short damageValue = 0;      // ダメージ値
        Vector2 pos;                // 描画座標
    };

    // ダメージUIの情報のリスト
    std::list<AddDamageUiInfo> damageUiList;
};
