#include "GrapeBossSingleState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossSingleState::GrapeBossSingleState(
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

void GrapeBossSingleState::Enter(void)
{
	PlayAttackAnim();
	targetPlayerIndex = GetTarget();
	SetCoolTime();
}

void GrapeBossSingleState::Update(void)
{
	if (targetPlayerIndex < 0 || targetPlayerIndex >= (int)MSG_SENDER_ID::Max || playerPos[targetPlayerIndex] == nullptr) { return; }

	// アニメーションが終わっていたら
	if (IsAnimeEnd()) {
		// 生成数まで回す
		for (int i = 0; i < WeaponPoolCounts[(int)WeaponType::SingleBomb]; i++) {

			// スタート位置の設定
			Vector3 spawnPosition = Vector3(playerPos[targetPlayerIndex]->x, HEIGHT, playerPos[targetPlayerIndex]->z);
			bombPool[i].weaponActor->SetStartPos(spawnPosition);
			bombPool[i].weaponActor->SetColliderFlg(true);
			bombPool[i].weaponActor->SetViewPosCircle();

			// 起動を通知
			Net::GetIns().Send(MsgDataBossBombInform(MsgDataBossBombInform::INFORM_TYPE::SingleBomb, i, spawnPosition));

			// 起動
			bombPool[i].isActive = true;

		}
		// 全弾の起動後に一度だけ遷移する。
		DefaultChangeState();
	}
}

void GrapeBossSingleState::Exit(void)
{
}

void GrapeBossSingleState::AlwaysUpdate(void)
{
}
