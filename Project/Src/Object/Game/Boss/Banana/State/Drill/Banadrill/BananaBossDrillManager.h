#pragma once
#include "BananaBossBanadrill.h"

#include "../../../../../../../Manager/Net/NetWorkDefine.h"
#include "../../../../../../Common/ActorBase/ActorBase.h"

struct CharacterStats;

class BananaBossDrillManager : public ActorBase {
public:

	BananaBossDrillManager(
		const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
		const int& target,
		const CharacterStats& stats
	);
	~BananaBossDrillManager() = default;

	void Load(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	bool GetEnd(void) { return end; }

private:

	// 例外処理用
	static constexpr float EXIT_HEIGHT = -10.0f;

	static constexpr int DRILL_NUM = 3;

#pragma region 参照値

	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max];
	const int& target;
	const CharacterStats& stats;

#pragma endregion

	BananaBossBanadrill* drills[DRILL_NUM];

	// 複製用ハンドル
	int drillModel;
	int backModel;
	int frontModel;

	bool end;
};
