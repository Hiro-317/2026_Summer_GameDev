#include "NetWorkManager.h"

NetWorkManager* NetWorkManager::ins = nullptr;

NetWorkManager::NetWorkManager() :
	state(NetState::None),
    stateUpdate(),
	netHandle(),
    senderId(MSG_SENDER_ID::None),
    msgData()
{
}

void NetWorkManager::Init()
{
#pragma region 各状態別更新処理関数を関数ポインタ配列に格納
    stateUpdate[(int)NetState::None] = &NetWorkManager::NoneUpdate;
    stateUpdate[(int)NetState::Hosting] = &NetWorkManager::HostingUpdate;
    stateUpdate[(int)NetState::Connecting] = &NetWorkManager::ConnectingUpdate;
    stateUpdate[(int)NetState::Connected] = &NetWorkManager::ConnectedUpdate;
    stateUpdate[(int)NetState::Error] = &NetWorkManager::ErrorUpdate;
#pragma endregion
}

void NetWorkManager::Update()
{
    // 状態に合わせた更新処理関数を呼び出す
    (this->*stateUpdate[(int)state])();
}

void NetWorkManager::Release()
{
    for (int handle : netHandle) { CloseNetWork(handle); }
    netHandle.clear();
    
    for (int dataType = 0; dataType < (int)MSG_DATA_TYPE::Max; dataType++) {
        for (int senderId = 0; senderId < (int)MSG_SENDER_ID::Max; senderId++) {
            for (void* ptr : msgData[dataType][senderId]) { if (ptr) { delete ptr; } }
            msgData[dataType][senderId].clear();
        }
    }

    senderId = MSG_SENDER_ID::None;

    SetWindowText("");
}

#pragma region 各状態更新処理

// 未接続：何もしない
void NetWorkManager::NoneUpdate(void) {}

// ホストとして待機中
void NetWorkManager::HostingUpdate(void)
{
    for (int i = 0; i < (int)MSG_SENDER_ID::Max; i++) {
        // 新しい接続があるかチェック
        int newHandle = GetNewAcceptNetWork();
        if (newHandle == -1) { break; }
        else {
            // ハンドルを保持
            netHandle.emplace_back(newHandle);

            // 接続人数を更新
            connectNum = (int)netHandle.size();

            // IDを送信する
            MsgDataSenderId data((MSG_SENDER_ID)connectNum);
            data.header.senderId = senderId;
            NetWorkSend(netHandle.back(), &data, sizeof(MsgDataSenderId));
        }
    }
}

// クライアントとして接続中
void NetWorkManager::ConnectingUpdate(void)
{
    // 接続が完了したかチェック
    for (int handle : netHandle) {
        if (GetNetWorkAcceptState(handle) == 1) { state = NetState::Connected; }
    }
}

// 接続完了・プレイ中
void NetWorkManager::ConnectedUpdate(void)
{
    // 接続チェック
    for (int handle : netHandle) {
        if (GetNetWorkAcceptState(handle) == 0) {
            state = NetState::Error;
            return;
        }

        for (char i = 0; i < RECEPTION_MAX_NUM; i++) {
            // データの受信チェック
            if (GetNetWorkDataLength(handle) <= 0) { break; }

            // 一時格納用メモリ領域を確保
            char buffer[1024];

            // バッファから削除せずに先頭のヘッダーだけ取得し、どの情報かを判別する
            NetWorkRecvToPeek(handle, buffer, sizeof(MsgDataHeader));
            MsgDataHeader* headerData = reinterpret_cast<MsgDataHeader*>(buffer);

            // 受信判定
            switch (headerData->dataType) {
            case MSG_DATA_TYPE::None: { NetWorkRecv(handle, buffer, sizeof(MsgDataHeader)); break; }
            case MSG_DATA_TYPE::SenderId: { MsgDataRecv<MsgDataSenderId>(handle, headerData->senderId); break; }
            case MSG_DATA_TYPE::ConnectNum: { MsgDataRecv<MsgDataConnectNum>(handle, headerData->senderId); break; }
            case MSG_DATA_TYPE::SystemInform: { MsgDataRecv<MsgDataSystemInform>(handle, headerData->senderId); break; }
            case MSG_DATA_TYPE::PlayerTrans: { MsgDataRecv<MsgDataPlayerTrans>(handle, headerData->senderId); break; }
            case MSG_DATA_TYPE::PlayerInput: { MsgDataRecv<MsgDataPlayerInput>(handle, headerData->senderId); break; }
            case MSG_DATA_TYPE::PlayerDamage: { MsgDataRecv<MsgDataPlayerDamage>(handle, headerData->senderId); break; }
            }
        }
    }
}

// エラーや切断
void NetWorkManager::ErrorUpdate(void)
{
    for (int handle : netHandle) {

    }
    Release();
    state = NetState::None;
}

#pragma endregion