#include "NetWorkManager.h"

NetWorkManager* NetWorkManager::ins = nullptr;

NetWorkManager::NetWorkManager() :
    state(NetState::None),
    stateUpdate(),

    hostAddressProvider(nullptr),

    senderId(MSG_SENDER_ID::None),
    connectInfo(),
    connectStatus(),

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
    // 生成されていたら
    if (hostAddressProvider) { hostAddressProvider->Update(); }
    // 状態に合わせた更新処理関数を呼び出す
    (this->*stateUpdate[(int)state])();
}

void NetWorkManager::Release()
{
    // ホストアドレス取得クラスの削除
    if (hostAddressProvider) {
        hostAddressProvider->End();
        delete hostAddressProvider;
        hostAddressProvider = nullptr;
    }

    // 接続されているハンドルを全て解放
    for (ConnectInfo info : connectInfo) { CloseNetWork(info.handle); }
    connectInfo.clear();

    // 接続状況をクリア
    connectStatus.Reset();
    
    // 格納保持されたままのたまった受信データを全て 解放/削除 する
    for (int dataType = 0; dataType < (int)MSG_DATA_TYPE::Max; dataType++) {
        for (int senderId = 0; senderId < (int)MSG_SENDER_ID::Max; senderId++) {
            for (void* ptr : msgData[dataType][senderId]) {
                if (!ptr) { continue; }
                switch ((MSG_DATA_TYPE)dataType) {
                case MSG_DATA_TYPE::SenderId: { delete static_cast<MsgDataSenderId*>(ptr); break; }
                case MSG_DATA_TYPE::SystemInform: { delete static_cast<MsgDataSystemInform*>(ptr); break; }
                case MSG_DATA_TYPE::PlayerTrans: { delete static_cast<MsgDataPlayerTrans*>(ptr); break; }
                case MSG_DATA_TYPE::PlayerAnimeStep: { delete static_cast<MsgDataPlayerAnimeStep*>(ptr); break; }
                case MSG_DATA_TYPE::PlayerInput: { delete static_cast<MsgDataPlayerInput*>(ptr); break; }
                case MSG_DATA_TYPE::PlayerDamage: { delete static_cast<MsgDataPlayerDamage*>(ptr); break; }
                }
            }
            msgData[dataType][senderId].clear();
        }
    }

    // 状態を「未接続」に戻す
    state = NetState::None;

    // 送信者IDを未設定に戻す
    senderId = MSG_SENDER_ID::None;

    // ウィンドウテキストを消す
    SetWindowText("");
}

#pragma region 各状態更新処理

// 未接続：何もしない
void NetWorkManager::NoneUpdate(void) {}

// ホストとして待機中
void NetWorkManager::HostingUpdate(void)
{
    // 確立した接続の確認
    for (std::vector<ConnectInfo>::iterator info = connectInfo.begin(); info != connectInfo.end(); ) {
        // 接続チェック
        if (GetNetWorkAcceptState(info->handle) == 0) {
            // 接続が切れていた場合の処理

            // ハンドルを破棄
            CloseNetWork(info->handle);

            // 接続状況を更新
            connectStatus.RemoveMember();
            Send(MsgDataConnectStatus(connectStatus));

            // 接続情報を消去
            info = connectInfo.erase(info);

            // クライアントの送信IDを更新
            SenderIdSend();
        }
        // 正常なら次の要素へ
        else { ++info; }
    }

    // 新規接続検出
    for (int i = connectStatus.EntryCount(); i < (int)MSG_SENDER_ID::Max; i++) {
        // 新しい接続があるかチェック
        int newHandle = GetNewAcceptNetWork();
        if (newHandle == -1) { break; }
        else {
            // ハンドルを保持 / 接続状況を更新
            connectInfo.emplace_back(newHandle, connectStatus.AddMember());

            // 送信IDを送る
            MsgDataSenderId data(connectInfo.back().senderId);
            data.header.senderId = senderId;
            NetWorkSend(connectInfo.back().handle, &data, sizeof(MsgDataSenderId));

            // 接続状況を送る
            Send(MsgDataConnectStatus(connectStatus));
        }
    }

    // 受信処理を行う
    ConnectedUpdate();
}

// クライアントとして接続中
void NetWorkManager::ConnectingUpdate(void)
{
    IPDATA hostAddress;
    if (!hostAddressProvider->GetHostAddress(hostAddress)) { return; }

    // 接続
    int newHandle = ConnectNetWork(hostAddress, PORT_NUMBER);

    // 接続チェック
    if (newHandle != -1) {
        // 成功

        hostAddressProvider->End();
        delete hostAddressProvider;
        hostAddressProvider = nullptr;

        // ハンドルを保存
        connectInfo.emplace_back(newHandle, HOST_SENDER_ID);

        // 状態を「接続完了・プレイ中」に設定する
        state = NetState::Connected;
    }
}

// 接続完了・プレイ中
void NetWorkManager::ConnectedUpdate(void)
{
    for (const ConnectInfo& info : connectInfo) {
        // 接続チェック
        if (GetNetWorkAcceptState(info.handle) == 0) {
            if (connectStatus.IsAlive(info.senderId)) { state = NetState::Error; }
            continue;
        }

        for (char i = 0; i < RECEPTION_MAX_NUM; i++) {
            // データの受信チェック
            if (GetNetWorkDataLength(info.handle) <= 0) { break; }

            // 一時格納用メモリ領域を確保
            char buffer[1024];

            // バッファから削除せずに先頭のヘッダーだけ取得し、どの情報かを判別する
            NetWorkRecvToPeek(info.handle, buffer, sizeof(MsgDataHeader));
            MsgDataHeader* headerData = reinterpret_cast<MsgDataHeader*>(buffer);

            // 受信判定
            switch (headerData->dataType) {
            case MSG_DATA_TYPE::None:           { NetWorkRecv(info.handle, buffer, sizeof(MsgDataHeader)); break; }
            case MSG_DATA_TYPE::SenderId:       { MsgDataSenderIdRecv(info.handle); break; }
            case MSG_DATA_TYPE::ConnectStatus:  { MsgDataConnectStatusRecv(info.handle); break; }
            case MSG_DATA_TYPE::SystemInform:   { MsgDataRecv<MsgDataSystemInform>(info.handle, headerData->senderId); break; }
            case MSG_DATA_TYPE::PlayerTrans:    { MsgDataRecv<MsgDataPlayerTrans>(info.handle, headerData->senderId); break; }
            case MSG_DATA_TYPE::PlayerAnimeStep:{ MsgDataRecv<MsgDataPlayerAnimeStep>(info.handle, headerData->senderId); break; }
            case MSG_DATA_TYPE::PlayerInput:    { MsgDataRecv<MsgDataPlayerInput>(info.handle, headerData->senderId); break; }
            case MSG_DATA_TYPE::PlayerDamage:   { MsgDataRecv<MsgDataPlayerDamage>(info.handle, headerData->senderId); break; }
            }
        }
    }
}

// エラーや切断
void NetWorkManager::ErrorUpdate(void)
{
    // ホスト
    if (IsHost()) {
        for (ConnectInfo& info : connectInfo) {
            if (GetNetWorkAcceptState(info.handle) == 0) {
                // ハンドルを破棄
                CloseNetWork(info.handle);
                info.handle = -1;

                // 接続状況を更新
                connectStatus.LostMember(info.senderId);
                Send(MsgDataConnectStatus(connectStatus));
            }
        }

        // 状態を戻す
        state = NetState::Connected;
    }
    // クライアント
    else { Release(); clsDx(); }
}

#pragma endregion