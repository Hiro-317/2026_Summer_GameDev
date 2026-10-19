#include "PlayerManager.h"

#include "../../../../Manager/Net/NetWorkManager.h"

#include "../PlayerBase.h"

PlayerManager::PlayerManager() : playerInfoRef() {}

void PlayerManager::Load(void)
{
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		
		// プレイヤーを生成
		playerInfoRef.emplace_back(PlayerFactory::CreatePlayer((MSG_SENDER_ID)id));
	}
	
	for (PlayerInfo& own : playerInfoRef) {
		for (PlayerInfo& other : playerInfoRef) {
			// 自身以外のプレイヤーの座標を渡す
			if (own.instance->GetControlSenderId() != other.instance->GetControlSenderId()) {
				own.instance->SetOtherPlayerTrans(&other.instance->GetTrans());
			}
		}
	}

	// 下位アクターに追加
	for (PlayerInfo& player : playerInfoRef) { AddChildActor(player.instance); }
}

bool PlayerManager::IsPlayerAllDeath(void) const
{
	for (const PlayerInfo& info : playerInfoRef) {
		if (!info.instance->GetIsDeath()) { return false; }
	}
	return true;
}

void PlayerManager::SetBossPos(const Vector3* bossPos)
{
	for (PlayerInfo& info : playerInfoRef) { info.instance->SetBossPos(bossPos); }
}
