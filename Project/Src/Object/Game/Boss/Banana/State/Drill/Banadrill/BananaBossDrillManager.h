#pragma once
#include "BananaBossBanadrill.h"

#include "../../../../../../../Manager/Net/NetWorkDefine.h"
#include "../../../../../../Common/ActorBase/ActorBase.h"

struct CharacterStats;

class BananaBossDrillManager : public ActorBase {
public:

	BananaBossDrillManager(const MSG_SENDER_ID& operatorSenderId, const CharacterStats& stats);
	~BananaBossDrillManager() = default;

	void Load(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// 例外処理用
	static constexpr float EXIT_HEIGHT = -10.0f;

	static constexpr int DRILL_NUM = 3;

#pragma region 参照値

	const MSG_SENDER_ID& operatorSenderId;
	const CharacterStats& stats;

#pragma endregion

	BananaBossBanadrill* drills[DRILL_NUM];

	// 複製用ハンドル
	int drillModel;
	int backModel;
	int frontModel;
};
