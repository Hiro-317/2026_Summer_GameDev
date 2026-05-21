#include "PlayerManager.h"

#include "../../../../Manager/Net/NetWorkManager.h"

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
}

void PlayerManager::Load(void)
{
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		playerInfo.emplace_back(PlayerFactory::CreatePlayer((MSG_SENDER_ID)id));
	}
	for (PlayerInfo& info : playerInfo) {
		info.instance->Load();
	}
}

void PlayerManager::Init(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->Init();
	}
}

void PlayerManager::Update(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->Update();
	}
}

void PlayerManager::SendUpdate(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->SendUpdate();
	}
}

void PlayerManager::Draw(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->Draw();
	}
}

void PlayerManager::UiDraw(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->UiDraw();
	}
}

void PlayerManager::Release()
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->Release();
	}
}

void PlayerManager::ReceptionUpdate(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->ReceptionUpdate();
	}
}

void PlayerManager::AlphaDraw(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->AlphaDraw();
	}
}