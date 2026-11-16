#include "GrapeBossStalkerState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossStalkerState::GrapeBossStalkerState(
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	BombPoolEntry* bombPool,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayAttackAnim,
	const std::function<float(void)> GetAnimPlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
) :
	playerPos(playerPos),
	bombPool(bombPool),
	GetTarget(GetTarget),
	PlayAttackAnim(PlayAttackAnim),
	GetAnimPlayRatio(GetAnimPlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void GrapeBossStalkerState::Enter(void)
{
	PlayAttackAnim();
	isSpawningBombs = false;
	targetPlayerIndex = GetTarget();
	SetCoolTime();
	spawnedBombCount = 0;
	spawnIntervalFrames = 0;
}

void GrapeBossStalkerState::Update(void)
{
	if (targetPlayerIndex < 0 || targetPlayerIndex >= (int)MSG_SENDER_ID::Max || playerPos[targetPlayerIndex] == nullptr) { return; }

	// アニメーションが終わっていたらフラグを立てる
	if (!isSpawningBombs && IsAnimeEnd()) {

		PlayAttackAnim();
		isSpawningBombs = true;
	}

	// フラグを立ってるなら
	if (isSpawningBombs) {
		// 間隔をとる
		if (spawnIntervalFrames >= DISP_TIME) {
			spawnIntervalFrames = 0;
			// 生成数まで回す
			if (spawnedBombCount < WeaponPoolCounts[(int)WeaponType::StalkerBomb]) {

				// スタート位置の設定
				Vector3 spawnPosition = Vector3(playerPos[targetPlayerIndex]->x, HEIGHT, playerPos[targetPlayerIndex]->z);
				bombPool[spawnedBombCount].weaponActor->SetStartPos(spawnPosition);
				bombPool[spawnedBombCount].weaponActor->SetColliderFlg(true);
				bombPool[spawnedBombCount].weaponActor->SetViewPosCircle();

				// 起動を通知
				Net::GetIns().Send(MsgDataBossBombInform(MsgDataBossBombInform::INFORM_TYPE::StalkerBomb, spawnedBombCount, spawnPosition));

				// 起動
				bombPool[spawnedBombCount].isActive = true;

				spawnedBombCount++;
			}
			// 全生成したらデフォルトに戻る
			else {
				DefaultChangeState();
			}
		}
		else {
			spawnIntervalFrames++;
		}
	}
}

void GrapeBossStalkerState::Exit(void)
{
}

void GrapeBossStalkerState::AlwaysUpdate(void)
{
}
