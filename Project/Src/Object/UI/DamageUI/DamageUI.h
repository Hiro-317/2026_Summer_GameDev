#pragma once
#include "../UI_Base.h"
#include "../../../Application/Application.h"

#include <list>

#include "../../../Common/Vector2.h"
#include "../../../Manager/Font/FontManager.h"

class DamageUI : public UI_Base
{
public:

	DamageUI();
	~DamageUI()override;

	void Load(void)override;
	void SubInit(void)override;
	void SubUpdate(void)override;
	void SubDraw(void)override;
	void SubRelease(void)override;

	short DamageSetting(const short damage);

private:

	static const int DAMAGE_UI_ALIVE_TIME = 60;

	struct AddDamageUiInfo
	{
		// 生存時間の更新
		bool AliveUpdate(int value = 1) {
			if (aliveTime < value) { return false; }
			aliveTime -= value;
			if (aliveTime <= 0) { return false; }
			return true;
		}

		// 描画
		void Draw(void) const
		{
			float t = (float)(DAMAGE_UI_ALIVE_TIME - aliveTime);

			float moveY = t * (2.0f - (t / DAMAGE_UI_ALIVE_TIME));

			DrawFormatStringToHandle(
				App::SCREEN_SIZE_X_HALF,
				App::SCREEN_SIZE_Y_HALF + 40 - moveY,
				0xffff00,
				Font::GetIns().GetFont(FontKinds::DEFAULT_64),
				"%d",
				damageValue
			);
		}

		short aliveTime;
		short damageValue;
	};

	std::list<AddDamageUiInfo> damageUiList;

};
