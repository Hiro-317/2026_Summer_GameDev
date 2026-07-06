#include "GrapeBossStraightState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"
#include "../../Weapon/Straight/GrapeBossStraight.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossStraightState::GrapeBossStraightState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	Vector3& pos, Vector3& angel,
	BombType* bombType,
	const std::vector<const Vector3*> playerPos,
	const std::function<int(void)> GetTarget
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), angle(angle),
	bombType(bombType),
	playerPos(playerPos),
	GetTarget(GetTarget)
{
}

void GrapeBossStraightState::Enter(void)
{
	target = GetTarget();
}

void GrapeBossStraightState::Update(void)
{
	// 正面から30度ずらした一発目
	Vector3 ang = angle.y - Deg2Rad(30.0f);

	// 生成数まで回す
	for (int i = 0; i < WeponDuplicateNum[(int)WeaponType::Straight]; i++) {

		// キャストさせる
		auto ins = dynamic_cast<GrapeBossStraight*>(bombType[i].weaponIns);
		
		// スタート位置の設定
		ins->SetStartPos(pos);
		// 進む向きを設定（ここのためにキャスト）
		ins->SetStartDir(ang);
		// 起動
		bombType[i].live = true;
		
		//次の周のため角度をずらす
		ang += Deg2Rad(30.0f);
	}
}

void GrapeBossStraightState::Exit(void)
{
}

void GrapeBossStraightState::AlwaysUpdate(void)
{
}
