#pragma once

#include <DxLib.h>

class PlayerSkillUI;
class PlayerStaminaUI;
class PlayerHpUI;

class UI_Manager
{
public:

	UI_Manager();
	~UI_Manager();

	void Load(void);
	void Init(void) {};
	void Update(void);
	void Draw(void);
	void Release(void);

	void AddSkillUI();

private:

};
