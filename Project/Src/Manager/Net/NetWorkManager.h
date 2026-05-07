#pragma once

#include <DxLib.h>

#include <vector>

#include <unordered_map>
#include <deque>
#include <typeindex>

#include <functional>

#include "NetWorkDefine.h"

class NetWorkManager
{
private:
	// インスタンス
	static NetWorkManager* ins;

	// コンストラクタ/デストラクタ
	NetWorkManager();
	~NetWorkManager() = default;

	static constexpr int PORT_NUMBER = 54321;

public:
#pragma region シングルトン定義
	// 生成/初期化
	static void CreateIns(void) { if (ins == nullptr) { ins = new NetWorkManager(); ins->Init(); } }
	// 取得
	static NetWorkManager& GetIns(void) { return *ins; }
	// 終了/削除
	static void DeleteIns(void) { if (ins != nullptr) { ins->Release(); delete ins; } }

	// コピー・ムーブ操作を禁止
	NetWorkManager(const NetWorkManager&) = delete;
	NetWorkManager& operator=(const NetWorkManager&) = delete;
	NetWorkManager(NetWorkManager&&) = delete;
	NetWorkManager& operator=(NetWorkManager&&) = delete;
#pragma endregion

	// ホストの送信ID
	static constexpr MSG_SENDER_ID HOST_SENDER_ID = MSG_SENDER_ID::P1;

	// 接続状態の列挙型定義
	enum class NetState {
		None,       // 未接続
		Hosting,    // ホストとして待機中
		Connecting, // クライアントとして接続中
		Connected,  // 接続完了・プレイ中
		Error,      // 切断やエラー

		Max,
	};

	// 更新処理
	void Update(void);

#pragma region 共通操作
	/// <summary>
	/// データを送信
	/// </summary>
	/// <param name="data">送信するデータ構造体</param>
	/// <param name="senderId"></param>
	template<typename MsgData>
	void Send(const MsgData& data, MSG_SENDER_ID senderId = MSG_SENDER_ID::None) {
		// 接続先があるか
		if (netHandle.empty()) { return; }

		// コピーしてヘッダーを適切なものに書き換える
		MsgData sendData = data;
		sendData.header.senderId = (senderId == MSG_SENDER_ID::None) ? this->senderId : senderId;

		// 送信
		for (int handle : netHandle) { NetWorkSend(handle, &sendData, sizeof(MsgData)); }
	}

	// イベント通知を送信（Send()をラッピングした関数）
	void EventInformSend(MSG_DATA_SYSTEM_INFORM eventInform) { Send(MsgDataSystemInform(eventInform)); }

	// データを取得する
	template <typename DataType>
	DataType* GetMsgData(MSG_SENDER_ID senderId = HOST_SENDER_ID) {
		// 参照を取得
		std::deque<void*>& queue = msgData[(int)DataType::DATA_TYPE][(int)senderId];

		// 受信データがあるかどうか
		if (queue.empty()) { return nullptr; }

		// データのポインタをコピーする
		DataType* data = static_cast<DataType*>(queue.front());

		// 配列から削除する
		queue.pop_front();

		// 返す
		return data;
	}

	// 状態取得
	NetState GetState(void)const { return state; }

	// ホストかどうか
	bool IsHost(void)const { return senderId == HOST_SENDER_ID; }

	// 自身の送信ID
	MSG_SENDER_ID GetSenderId(void)const { return senderId; }

	// 接続数取得
	int GetConnectNum(void)const { return connectNum; }

	// 切断
	void Disconnected(void) { Release(); }
#pragma endregion 共通操作

#pragma region ホスト操作
	// ホストとして受付開始
	bool StartHost(void) {
		// 接続受付を開始する
		if (PreparationListenNetWork(PORT_NUMBER) == 0) {
			// 成功

			// 状態を「ホストとして待機中」に設定する
			state = NetState::Hosting;

			// 送信IDを設定
			senderId = HOST_SENDER_ID;

			// セッティング成功として「true」を返し終了
			return true;
		}

		// セッティング失敗として「false」を返し終了
		return false;
	}

	// 接続受付を終了し、「接続完了・プレイ中」の状態へ遷移する
	void CloseReception(void) {
		// 接続受付を終了する
		StopListenNetWork();

		// 状態を「接続完了・プレイ中」に設定
		state = NetState::Connected;
	}
#pragma endregion ホスト操作

#pragma region クライアント操作

	// クライアントとして接続
	bool ConnectClient(IPDATA ip) {

		// 接続
		int newHandle = ConnectNetWork(ip, PORT_NUMBER);

		// 接続失敗として「false」を返し終了
		if (newHandle == -1) { return false; }

		// 成功～～～～～～～～～～～～～～～～～～～～

		// ハンドルを保存
		netHandle.emplace_back(newHandle);

		// 状態を「クライアントとして接続中」に設定する
		state = NetState::Connecting;

		// 接続成功として「true」を返し終了
		return true;

		// ～～～～～～～～～～～～～～～～～～～～成功
	}

	// 自身の送信IDを設定
	void SetSenderId(MSG_SENDER_ID senderId) { this->senderId = senderId; }

	// 接続人数を設定
	void SetConnectNum(int connectNum) { this->connectNum = connectNum; }

#pragma endregion クライアント操作


private:
	// 初期化処理
	void Init(void);
	// 終了処理
	void Release(void);

	// 現在の状態
	NetState state;

#pragma region 各状態更新処理
	// 未接続(更新処理)
	void NoneUpdate(void);

	// ホストとして待機中(更新処理)
	void HostingUpdate(void);

	// クライアントとして接続中(更新処理)
	void ConnectingUpdate(void);
	
	// 接続完了・プレイ中(更新処理)
	void ConnectedUpdate(void);

	// 切断やエラー(更新処理)
	void ErrorUpdate(void);
#pragma endregion

	// 上記各状態(更新処理)を格納する関数ポインタ配列
	void (NetWorkManager::*stateUpdate[(int)NetState::Max])(void);

	// ネットハンドル
	std::vector<int> netHandle;

	// 送信ID
	MSG_SENDER_ID senderId;

	// 接続人数
	int connectNum;

	// 受け取った情報のポインタを格納する配列
	std::deque<void*> msgData[(int)MSG_DATA_TYPE::Max][(int)MSG_SENDER_ID::Max];

	// データのポインタを配列へ格納する関数
	template <typename DataType>
	void MsgDataRecv(int netHandle, MSG_SENDER_ID senderId) {
		DataType* newData = new DataType();
		NetWorkRecv(netHandle, newData, sizeof(DataType));
		msgData[(int)DataType::DATA_TYPE][(int)senderId].emplace_back(newData);
	}

	// 1フレームで受信できる最大情報数
	const char RECEPTION_MAX_NUM = 10;
};
using Net = NetWorkManager;
using NetState = Net::NetState;