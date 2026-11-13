#include "BananaBossDrillManager.h"

#include "BananaBossBanadrill.h"

#include "../../../../../../../Manager/Sound/SoundManager.h"


BananaBossDrillManager::BananaBossDrillManager(
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	const int& target,
	const CharacterStats& stats
):
	playerPos(playerPos), target(target), stats(stats)
{
	// 複製ハンドル
	drillModel = MV1LoadModel("Data/Model/Character/Banana/Banadrill.mv1");
	frontModel = MV1LoadModel("Data/Model/Range/CircleRangeFront.mv1");
	backModel = MV1LoadModel("Data/Model/Range/CircleRangeBack.mv1");

	for (int i = 0; i < DRILL_NUM; i++) {
		drills[i] = new BananaBossBanadrill(drillModel, frontModel, backModel, stats);
		AddChildActor(drills[i]);
	}
}

void BananaBossDrillManager::Update()
{
	bool anyDrilling = false;

	// 武器の更新
	for (int i = 0; i < DRILL_NUM; i++) {
		if (drills[i]->GetLive()) {
			drills[i]->Update();

			// 最後じゃない場合
			if (i < DRILL_NUM - 1) {
				// 次のものを起動するかどうか
				if (drills[i]->GetBoot()) {
					drills[i + 1]->SetLive(true);
					drills[i + 1]->SetPos(*playerPos[target]);
				}
			}
			// 最後の場合
			else {
				if (drills[i]->GetBoot()) {
					end = true;
				}
			}
			drills[i]->ResetBoot();
		}
		drills[i]->EffectUpdate();
		anyDrilling = anyDrilling || drills[i]->IsDrilling();
	}
	// 全本の更新後に判定し、別のドリルが回転中なら音を止めない。
	if (anyDrilling && !drillSoundPlaying) {
		Snd::GetIns().Play("Drill");
		drillSoundPlaying = true;
	}
	else if (!anyDrilling && drillSoundPlaying) {
		Snd::GetIns().Stop("Drill");
		drillSoundPlaying = false;
	}
}

void BananaBossDrillManager::Release()
{
	// シーン終了時にもループ音を残さない。
	if (drillSoundPlaying) {
		Snd::GetIns().Stop("Drill");
		drillSoundPlaying = false;
	}

	// 武器の解放
	MV1DeleteModel(drillModel);
	MV1DeleteModel(frontModel);
	MV1DeleteModel(backModel);
}

void BananaBossDrillManager::Start(void)
{
	end = false;
	drills[0]->SetLive(true);
	drills[0]->SetPos(*playerPos[target]);
}
