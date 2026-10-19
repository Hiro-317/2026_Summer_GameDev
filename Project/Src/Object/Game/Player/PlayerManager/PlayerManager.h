#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

#include "PlayerFactory.h"

#include "../../../../Manager/Net/SenderID_Define.h"

class PlayerBase;

class PlayerManager : public ActorBase
{
public:

	PlayerManager();	// コンストラクタ
	~PlayerManager()override = default;	// デストラクタ

	void Load(void)override;

	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;

	const PlayerBase* GetPlayerIns(const MSG_SENDER_ID id)const { return playerInfoRef[(int)id].instance; }

	bool IsPlayerAllDeath(void)const;

	// ボスの座標のポインタをセット
	void SetBossPos(const Vector3* bossPos);

private:

	const int PLAYER_MAX = Net::GetIns().GetConnectStatus().EntryCount();

	std::vector<PlayerInfo> playerInfoRef;
};

