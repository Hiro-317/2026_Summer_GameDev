#include "GrapeBossStraightState.h"

#include "../../../../../../Utility/Utility.h"

#include "../../Weapon/GrapeBossWeaponManager.h"
#include "../../Weapon/Straight/GrapeBossStraight.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

GrapeBossStraightState::GrapeBossStraightState(
	Transform& bossTrans,
	BombPoolEntry* bombPool,
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayAttackAnim,
	const std::function<float(void)> GetAnimPlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
) :
	bossTrans(bossTrans),
	bombPool(bombPool),
	playerPos(playerPos),
	GetTarget(GetTarget),
	PlayAttackAnim(PlayAttackAnim),
	GetAnimPlayRatio(GetAnimPlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void GrapeBossStraightState::Enter(void)
{
	targetPlayerIndex = GetTarget();
	PlayAttackAnim();
	isAttackPending = true;
	SetCoolTime();
}

void GrapeBossStraightState::Update(void)
{
	// アニメーションの再生割合で生成
	if (isAttackPending) {
		if (GetAnimPlayRatio() >= RATIO) {
			// 正面から30度ずらした一発目
			float shotYaw = bossTrans.rotation.GetYaw() - Deg2Rad(30.0f);

			// 生成数まで回す
			for (int i = 0; i < WeaponPoolCounts[(int)WeaponType::Straight]; i++) {

				// キャストさせる
				auto straightBomb = dynamic_cast<GrapeBossStraight*>(bombPool[i].weaponActor);

				// スタート位置の設定
				Vector3 spawnPosition = Vector3(bossTrans.pos.x, HEIGHT, bossTrans.pos.z);
				straightBomb->SetStartPos(spawnPosition);
				// 進む向きを設定（ここのためにキャスト）
				Vector3 shotDirection = Vector3(sinf(shotYaw), 0.0f, cosf(shotYaw));
				straightBomb->SetStartDir(shotDirection);

				// 起動を通知
				Net::GetIns().Send(MsgDataBossBombInform(MsgDataBossBombInform::INFORM_TYPE::Straight, i, spawnPosition, shotDirection));

				// 起動
				bombPool[i].isActive = true;
				straightBomb->SetColliderFlg(true);

				//次の周のため角度をずらす
				shotYaw += Deg2Rad(30.0f);
			}
			//音を出す
			Snd::GetIns().Play("Throw");
			// 二度目は通さないようにフラグを折る
			isAttackPending = false;
		}
	}

	// アニメーションが終わっていたらステートを抜ける
	if (IsAnimeEnd()) {

		DefaultChangeState();
	}
}

void GrapeBossStraightState::Exit(void)
{
}

void GrapeBossStraightState::AlwaysUpdate(void)
{
}
