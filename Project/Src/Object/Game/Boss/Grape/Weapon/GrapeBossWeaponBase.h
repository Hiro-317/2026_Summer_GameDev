#pragma once

#include "../../../../Common/ActorBase/ActorBase.h"

#include "GrapeBossWeaponTypeDefine.h"

struct CharacterStats;

class GrapeBossWeaponBase : public ActorBase
{
public:
		
	GrapeBossWeaponBase(int model, int front, int back);
	~GrapeBossWeaponBase() = default;

	// 規定用のLoad
	virtual void Load(void) override {};
	// 行うロード
	virtual void Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats) = 0;

	// 攻撃が終わったか
	virtual bool IsEnd(void)const { return state == STATE::None; }

	// 
	void SetStartPos(const Vector3& pos) { trans.pos = pos; }

	// 予測線の設定(円形)
	void SetViewScaleCircle(float scale) { collFront.scale = scale; };

	void SetViewPosCircle(void) {

		collBack.pos = trans.pos;
		collBack.pos.y = VIEW_HEIGHT;

		collFront.pos = trans.pos;
		collFront.pos.y = VIEW_HEIGHT;
	};


protected:

	enum class STATE {
		None,

		Fall,

		Ground,

		Blast,
	};

	// 基本
	void SubDraw(void) override;

	// 予測線
	Transform collBack;
	Transform collFront;

	STATE state;

	// 行動カウント
	float count;

private:
#pragma region 定数定義

	static constexpr float VIEW_HEIGHT = 37.0f;

#pragma endregion
};
