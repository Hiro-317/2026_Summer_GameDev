#pragma once

#include "../CharaSelectObj.h"

class OrangeSelect : public CharaSelectObj
{
public:

	OrangeSelect();
	~OrangeSelect()override = default;

	void Load(void)override;

private:

	// ƒƒCƒ“ˆ—``````````````
	void CharactorInit(void)override;
	void CharactorUpdate(void)override;
	void CharactorDraw(void)override;
	void CharactorAlphaDraw(void)override;
	void CharactorRelease(void)override;
	// ```````````````````
};