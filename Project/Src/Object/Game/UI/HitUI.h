#pragma once

#include "../../Common/ActorBase/ActorBase.h"

#include <list>

#include "../../../Utility/Utility.h"

#include "../../../Manager/Font/FontManager.h"

class HitUI : public ActorBase
{
public:

    HitUI();
    ~HitUI() override {}

    void Load(void) override;       // ロード
    void Init(void) override;    // 初期化
    void Update(void) override;  // 更新


    /// <summary>
    /// ダメージ値の設定
    /// </summary>
    /// <param name="damage">表示するダメージ値</param>
    /// <param name="isCritical">会心ならば true / 非会心ならば false</param>
    void DamageSetting(const Vector3& collisionPos, const short damage, const bool isCritical = false);

    // ミス！を設定
    void MissSetting(const Vector3& collisionPos);

private:

    static constexpr float HIT_UI_ALIVE_TIME = 60.0f;
    static constexpr float DAMAGE_UI_BOUNCE_START_TIME = HIT_UI_ALIVE_TIME / 3;
    static constexpr float DAMAGE_UI_BOUNCE_TIME = HIT_UI_ALIVE_TIME - DAMAGE_UI_BOUNCE_START_TIME;
    static constexpr float DAMAGE_UI_BOUNCE_POWER = 12.0f;

    // ダメージ描画用構造体
    struct AddHitUiInfo
    {
        // 表示するテキスト
        std::string text;
        // テキストのカラー
        int color = 0xffffff;
        // テキストの生存時間
        float aliveTime = 0.0f;
        // テキストの座標
        Vector2 pos;

        // ダメージ描画の時間更新処理
        bool AliveUpdate(void);

        // 描画処理
        void Draw(void)const;
    };

    /// <summary>
    /// 描画するテキストと色を設定し、描画を開始する
    /// </summary>
    /// <param name="text">表示する文字の指定</param>
    /// <param name="color">表示する文字の色指定</param>
    void AddPopup(const Vector2 pos, const std::string& text, int color);

    // 描画
    void SubDraw(void) override;
    // 解放
    void SubRelease(void) override;

    std::list<AddHitUiInfo> hitUiList;
};
