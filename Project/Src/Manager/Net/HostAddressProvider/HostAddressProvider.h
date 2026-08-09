#pragma once

#include "../../../pch.h"

#include <cstdint>
#include <string>

//==============================================================
// 接続先
//==============================================================
struct NetEndpoint
{
	std::string ip;
	enet_uint16 port = 0;

	bool IsValid(void) const
	{
		return !ip.empty() && port != 0;
	}
};

//==============================================================
// 接続経路
//==============================================================
enum class CONNECTION_ROUTE
{
	None,

	// 同じPC
	Loopback,

	// 同じLAN
	Local,

	// インターネット越し
	Public,

	// 将来用：リレー
	Relay,
};

//==============================================================
// HostAddressProvider
//
// ホストの「IPだけ」を探すクラスではなく、
//
//   IP
//   Port
//   接続経路
//
// を決定するクラス。
//==============================================================
class HostAddressProvider
{
public:

	enum class MODE
	{
		Host,
		Client,

		Max
	};

private:

	//==========================================================
	// 内部状態
	//==========================================================
	enum class STATE
	{
		// 初期化直後
		Initialize,

		// Oracleへ自身の情報を登録
		Register,

		// マッチング結果待ち
		WaitMatch,

		// NAT Hole Punch処理中
		Punch,

		// 接続先決定完了
		Complete,

		// エラー
		Error,
	};

public:

	//==========================================================
	// コンストラクタ
	//
	// mode
	//     Host / Client
	//
	// roomNumber
	//     あいことば
	//
	// enetHost
	//     NetWorkManagerが生成したENetHost
	//
	// localPort
	//     OSから自動割り当てされたENetのポート
	//==========================================================
	HostAddressProvider(
		MODE mode,
		unsigned short roomNumber,
		ENetHost* enetHost,
		enet_uint16 localPort
	);

	~HostAddressProvider() = default;

	//==========================================================
	// 更新
	//==========================================================
	void Update(void);

	//==========================================================
	// 終了
	//==========================================================
	void End(void);

	//==========================================================
	// クライアント用
	//
	// 最終的に決定した接続先を取得する。
	//
	// まだ検索中なら false。
	// 決定済みなら true。
	//==========================================================
	bool GetConnectEndpoint(
		NetEndpoint& endpoint,
		CONNECTION_ROUTE& route
	) const;

	//==========================================================
	// 状態確認
	//==========================================================
	bool IsComplete(void) const
	{
		return state == STATE::Complete;
	}

	bool IsError(void) const
	{
		return state == STATE::Error;
	}

	//==========================================================
	// ENetゲームポート
	//==========================================================
	enet_uint16 GetLocalPort(void) const
	{
		return localEndpoint.port;
	}

private:

	//==========================================================
	// マッチングサーバー
	//==========================================================

	// Oracle Cloud
	static constexpr const char* MATCH_SERVER_IP =
		"161.33.190.216";

	// Oracle側の制御用UDPポート
	static constexpr int MATCH_SERVER_PORT =
		50000;

	//==========================================================
	// タイムアウト等
	//==========================================================

	// サーバーへの再送間隔
	static constexpr int SERVER_SEND_COOLTIME =
		60;

	// Hole Punch送信間隔
	static constexpr int PUNCH_SEND_COOLTIME =
		10;

	// Hole Punch最大送信回数
	static constexpr int PUNCH_MAX_COUNT =
		60;

private:

	//==========================================================
	// 基本情報
	//==========================================================

	MODE mode;

	STATE state;

	unsigned short roomNumber;

	// このPCを識別するID
	std::string machineId;

	//==========================================================
	// ENet
	//==========================================================

	// NetWorkManagerが所有。
	// HostAddressProvider側ではdeleteしない。
	ENetHost* enetHost;

	//==========================================================
	// Oracle制御用ソケット
	//
	// ENetとは別。
	// マッチング情報のやり取り専用。
	//==========================================================
	int controlSocket;

	//==========================================================
	// 自分自身の接続情報
	//==========================================================

	// LAN内
	NetEndpoint localEndpoint;

	// Oracleから観測されたENet側Public Endpoint
	NetEndpoint publicEndpoint;

	//==========================================================
	// ホスト情報
	//==========================================================

	std::string hostMachineId;

	NetEndpoint hostLocalEndpoint;

	NetEndpoint hostPublicEndpoint;

	//==========================================================
	// 最終的にNetWorkManagerへ渡す接続先
	//==========================================================

	NetEndpoint connectEndpoint;

	CONNECTION_ROUTE connectionRoute;

	//==========================================================
	// カウンタ
	//==========================================================

	int serverSendCounter;

	int punchSendCounter;

	int punchCount;

private:

	//==========================================================
	// モード別更新
	//==========================================================

	void HostUpdate(void);

	void ClientUpdate(void);

	//==========================================================
	// Oracle関連
	//==========================================================

	// 自分自身をOracleへ登録
	void RegisterToServer(void);

	// Oracleからの受信
	void ReceiveServerMessage(void);

	// ENet自身のソケットからOracleへパケットを送り、
	// NAT後のPublic IP / PortをOracleに観測させる
	void SendEndpointProbe(void);

	//==========================================================
	// NAT Hole Punch
	//==========================================================

	// 相手のPublic Endpointへ、
	// ENet自身のソケットからUDPを送信
	void SendPunchPacket(
		const NetEndpoint& endpoint
	);

	//==========================================================
	// 接続先決定
	//==========================================================

	void DecideConnectionRoute(void);

	//==========================================================
	// PC / LAN情報
	//==========================================================

	// PC固有IDの取得・生成
	std::string GetOrCreateMachineId(void);

	// LAN内IPv4取得
	std::string GetLocalIPv4(void);

	//==========================================================
	// ユーティリティ
	//==========================================================

	bool SendControlMessage(
		const std::string& message
	);

	bool SendRawFromENet(
		const NetEndpoint& endpoint,
		const std::string& message
	);
};