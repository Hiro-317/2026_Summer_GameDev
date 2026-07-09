#pragma once

#include"../../SceneBase.h"

#include "../../../Manager/Net/NetWorkDefine.h"

class GamePause : public SceneBase
{
public:
	GamePause(MSG_SENDER_ID operatorSenderId);
	~GamePause();

	void Load(void)override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	enum class SELECT { YES, NO, MAX };
	SELECT nowSelect;
	int img[(int)SELECT::MAX];

	MSG_SENDER_ID operatorSenderId;
	bool isOperator;
};
