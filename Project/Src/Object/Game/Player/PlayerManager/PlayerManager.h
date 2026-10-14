#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

#include "PlayerFactory.h"

#include "../../../../Manager/Net/NetWorkManager.h"

class PlayerBase;

class PlayerManager : public ActorBase
{
public:

	PlayerManager();	// コンストラクタ
	~PlayerManager();	// デストラクタ

	void Load(void)override;

	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;

	const PlayerBase* GetPlayerIns(const MSG_SENDER_ID id)const { return playerInfoRef[(int)id].instance; }

	const bool IsPlayerAllDeath(void)const {
		for (const PlayerInfo& info : playerInfoRef) {
			if (!info.instance->GetIsDeath()) { return false; }
		}
		return true;
	}

	// ボスの座標のポインタをセット
	void SetBossPos(const Vector3* bossPos) {
		for (PlayerInfo& info : playerInfoRef) { info.instance->SetBossPos(bossPos); } 
	}

private:

	const int PLAYER_MAX = Net::GetIns().GetConnectStatus().EntryCount();

	std::vector<PlayerInfo> playerInfoRef;
};

