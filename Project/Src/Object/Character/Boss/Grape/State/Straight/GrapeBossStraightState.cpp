#include "GrapeBossStraightState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"
#include "../../Weapon/Straight/GrapeBossStraight.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossStraightState::GrapeBossStraightState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	Vector3& pos, Vector3& angle,
	const std::function<BombType*(void)> bombType,
	const std::vector<const Vector3*> playerPos,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayAttackAnim,
	const std::function<float(void)> GetAnimPlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), angle(angle),
	bombType(bombType),
	playerPos(playerPos),
	GetTarget(GetTarget),
	PlayAttackAnim(PlayAttackAnim),
	GetAnimPlayRatio(GetAnimPlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState)
{
}

void GrapeBossStraightState::Enter(void)
{
	target = GetTarget();
	PlayAttackAnim();
	first = true;
}

void GrapeBossStraightState::Update(void)
{
	// 正面から30度ずらした一発目
	Vector3 ang = angle.y - Deg2Rad(30.0f);

	// アニメーションの再生割合で生成
	if (first) {
		if (GetAnimPlayRatio() >= 0.3f) {
			// 生成数まで回す
			for (int i = 0; i < WeponDuplicateNum[(int)WeaponType::Straight]; i++) {

				// キャストさせる
				auto ins = dynamic_cast<GrapeBossStraight*>(bombType()[i].weaponIns);

				// スタート位置の設定
				ins->SetStartPos(Vector3(pos.x, HEIGHT, pos.z));
				// 進む向きを設定（ここのためにキャスト）
				ins->SetStartDir(ang);
				// 起動
				bombType()[i].live = true;

				//次の周のため角度をずらす
				ang += Deg2Rad(30.0f);
			}
			// 二度目は通さないようにフラグを折る
			first = false;
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
