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

	void Load(void) override {};
	void Update(void) override;
	void Release(void) override;

	bool GetEnd(void) { bool ed = end; end = false; return ed; }

	void Start(void);

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

	bool end = false;

	// 複数のドリルで一つのループ音を共有する。
	bool drillSoundPlaying = false;
};
