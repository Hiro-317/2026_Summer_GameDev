#pragma once
#include "../UI_Base.h"
#include "../../../Application/Application.h"

#include <list>
#include <string>

#include "../../../Common/Vector2.h"
#include "../../../Manager/Font/FontManager.h"
#include "../../../Utility/Utility.h"

class HitUI : public UI_Base
{
public:

    HitUI();
    ~HitUI() override;

    void Load(void) override;
    void SubInit(void) override;
    void SubUpdate(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;

    void DamageSetting(const short damage, const bool isCritical = false);
    void MissSetting();

private:

    static constexpr float DAMAGE_UI_ALIVE_TIME = 60.0f;
    static constexpr float DAMAGE_UI_BOUNCE_START_TIME = DAMAGE_UI_ALIVE_TIME / 3;
    static constexpr float DAMAGE_UI_BOUNCE_TIME = DAMAGE_UI_ALIVE_TIME - DAMAGE_UI_BOUNCE_START_TIME;
    static constexpr float DAMAGE_UI_BOUNCE_POWER = 12.0f;

    struct AddDamageUiInfo
    {
        bool AliveUpdate(int value = 1) {
            if (aliveTime < value) { return false; }
            aliveTime -= value;
            if (aliveTime <= 0) { return false; }
            return true;
        }

        void Draw(void) const
        {
            float elapsedTime = (float)(DAMAGE_UI_ALIVE_TIME - aliveTime);
            float drawPosY;

            if (elapsedTime < DAMAGE_UI_BOUNCE_START_TIME) {
                drawPosY = pos.y + DAMAGE_UI_BOUNCE_TIME - (elapsedTime * 3.0f);
            }
            else {  
                float bounceTime = elapsedTime - DAMAGE_UI_BOUNCE_START_TIME;
                float bounceRate = 1.0f - (bounceTime / DAMAGE_UI_BOUNCE_TIME);
                UtilityClamp(bounceRate, 0.0f, 1.0f);

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

            DrawFormatStringToHandle(
                (int)pos.x,
                (int)drawPosY, 
                color, 
                Font::GetIns().GetFont(FontKinds::HIT_UI_64), 
                text.c_str()
            );

            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        std::string text;
        int color = 0xffffff;
        short aliveTime = 0;
        Vector2 pos;
    };

    void AddPopup(
        const std::string& text,
        int color
    );

    std::list<AddDamageUiInfo> damageUiList;
};
