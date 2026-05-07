#pragma once

#include "../../Common/Vector3.h"

#include "../Input/KeyManager.h"

// データタイプ（<> <- 送信者）
enum class MSG_DATA_TYPE
{
	// 未設定
	None = -1,

	// <ホスト>ID
	SenderId,

	// <ホスト>接続人数
	ConnectNum,

	// <ホスト>システムイベント（シーン遷移など）
	SystemInform,

	// <ホスト/クライアント>プレイヤー：自分の座標/角度
	PlayerTrans,
	// <ホスト/クライアント>プレイヤー：自分の入力
	PlayerInput,
	// <ホスト>プレイヤー：被ダメ
	PlayerDamage,

	Max
};

// ID
enum class MSG_SENDER_ID { None = -1, P1, P2, P3, P4, Max };

// システムイベント列挙型定義
enum class MSG_DATA_SYSTEM_INFORM
{
	None = -1,

	// 接続完了通知
	Connected,

	// タイトルシーンへ遷移
	ChangeSceneTitle,
	// ゲームシーンへ遷移
	ChangeSceneGame,

	// ゲームポーズ
	GamePause,
	// ゲームポーズからゲームへ戻す
	GamePauseEnd,
	// ゲームポーズ中の操作、選択肢の切り替え
	GamePauseChoicesSwitch,

	// クリアシーンへ遷移
	ChangeSceneClear,

};

#pragma region 各送信構造体定義

// ヘッダー（全ての構造体の先頭に配置する）
struct MsgDataHeader
{
    // データの種類
    MSG_DATA_TYPE dataType;

    // 送信者
    MSG_SENDER_ID senderId;

    MsgDataHeader(MSG_DATA_TYPE dataType) :
        dataType(dataType),
        senderId(MSG_SENDER_ID::None)
    {
    }
};

// <ホスト>ID送信構造体
struct MsgDataSenderId
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::SenderId;

    // ヘッダー
    MsgDataHeader header;

    // 送信者ID
    MSG_SENDER_ID senderId;

    MsgDataSenderId(MSG_SENDER_ID senderId) :
        header(MSG_DATA_TYPE::SenderId),
        senderId(senderId)
    {
    }
    MsgDataSenderId() :
        header(MSG_DATA_TYPE::SenderId),
        senderId(MSG_SENDER_ID::None)
    {
    }
};

// <ホスト>接続人数送信構造体
struct MsgDataConnectNum
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::ConnectNum;

    // ヘッダー
    MsgDataHeader header;

    // 接続人数
    int connectNum;

    MsgDataConnectNum(int connectNum) :
        header(MSG_DATA_TYPE::ConnectNum),
        connectNum(connectNum)
    {
    }
    MsgDataConnectNum() :
        header(MSG_DATA_TYPE::ConnectNum),
        connectNum()
    {
    }
};

// <ホスト>システムイベント送信構造体
struct MsgDataSystemInform
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::SystemInform;

    // ヘッダー
    MsgDataHeader header;

    // イベント種類
    MSG_DATA_SYSTEM_INFORM inform;

    MsgDataSystemInform(MSG_DATA_SYSTEM_INFORM inform) :
        header(MSG_DATA_TYPE::SystemInform),
        inform(inform)
    {
    }
    MsgDataSystemInform() :
        header(MSG_DATA_TYPE::SystemInform),
        inform(MSG_DATA_SYSTEM_INFORM::None)
    {
    }
};

// <ホスト/クライアント>プレイヤー移動情報送信構造体
struct MsgDataPlayerTrans
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::PlayerTrans;

    // ヘッダー
    MsgDataHeader header;

    // 座標
    Vector3 pos;

    // 角度
    Vector3 angle;

    MsgDataPlayerTrans(const Vector3& pos, const Vector3& angle) :
        header(MSG_DATA_TYPE::PlayerTrans),
        pos(pos),
        angle(angle)
    {
    }
    MsgDataPlayerTrans() :
        header(MSG_DATA_TYPE::PlayerTrans),
        pos(),
        angle()
    {
    }
};

// <ホスト/クライアント>プレイヤー入力送信構造体
struct MsgDataPlayerInput
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::PlayerInput;

    // ヘッダー
    MsgDataHeader header;

    // キーの種類
    KEY_TYPE type;

    // キーの入力詳細
    Key::KEY_INFO keyInfo;

    MsgDataPlayerInput(KEY_TYPE type, const Key::KEY_INFO& keyInfo) :
        header(MSG_DATA_TYPE::PlayerInput),
        type(type),
        keyInfo(keyInfo)
    {
    }
    MsgDataPlayerInput() :
        header(MSG_DATA_TYPE::PlayerInput),
        type(),
        keyInfo()
    {
    }
};

// <ホスト>被ダメ送信構造体
struct MsgDataPlayerDamage
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::PlayerDamage;

    // ヘッダー
    MsgDataHeader header;

    // ダメージ量
    int damage;

    // 攻撃元の座標（ノックバックに使用）
    Vector3 pos;

    MsgDataPlayerDamage(int damage, const Vector3& pos) :
        header(MSG_DATA_TYPE::PlayerDamage),
        damage(damage),
        pos(pos)
    {
    }
    MsgDataPlayerDamage() :
        header(MSG_DATA_TYPE::PlayerDamage),
        damage(),
        pos()
    {
    }
};

#pragma endregion