#include "BananaBossDrillManager.h"

#include "BananaBossBanadrill.h"


BananaBossDrillManager::BananaBossDrillManager(
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	const int& target,
	const CharacterStats& stats
):
	playerPos(playerPos), target(target), stats(stats)
{
	// 複製ハンドル
	drillModel = MV1LoadModel("Data/Model/Character/Banan/Banadril");
	frontModel = MV1LoadModel("Data/Model/Range/CircleRangeFront");
	backModel = MV1LoadModel("Data/Model/Range/CircleRangeBack");

	for (int i = 0; i < DRILL_NUM; i++) {
		drills[i] = new BananaBossBanadrill(drillModel, frontModel, backModel, stats);
	}
}

void BananaBossDrillManager::Load(void)
{
	// 武器の初期化
	for (auto& drill : drills) drill->Load();
}

void BananaBossDrillManager::Update()
{
	int num = 0;
	// 武器の更新
	for (int i = 0; i < DRILL_NUM; i++) {
		if (drills[i]->Live()) {
			num++;
			drills[i]->Update();

			// 最後じゃない場合
			if (i < DRILL_NUM - 1) {
				// 次のものを起動するかどうか
				if (drills[i]->Boot()) {
					drills[i + 1]->SetLive(true);
					drills[i + 1]->SetPos(*playerPos[target]);
				}
			}
			// 最後の場合
			else {
				if (drills[i]->GetIsGround()) {
					end = true;
				}
			}
		}
	}
	// 生きているものがいないならフラグを折る
	if (num == 0) {
		end = false;
	}
}

void BananaBossDrillManager::Draw()
{
	// 武器の描画
	for (auto& drill : drills) {
		drill->Draw();
	}
}

void BananaBossDrillManager::Release()
{
	// 武器の解放
	for (auto& drill : drills) {
		drill->Release();
		drill = nullptr;
	}
	MV1DeleteModel(frontModel);
	MV1DeleteModel(backModel);
}