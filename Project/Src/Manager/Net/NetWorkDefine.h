#pragma once

#include "../../pch.h"

#include "../../Common/Vector3.h"

#include "../Input/KeyManager.h"

// データタイプ（<> <- 送信者）
enum class MSG_DATA_TYPE
{
    // 未設定
    None = -1,

    // <ホスト/クライアント>接続に関するシステムイベント（接続完了、切断など）
    ConnectInform,

    // <ホスト>ID
    SenderId,

    // <ホスト>接続状況
    ConnectStatus,

    // <ホスト>システムイベント（シーン遷移など）
    SystemInform,

    // <ホスト/クライアント>キャラセレクト
	CharaSelect,
	// <クライアント>ロビーでの準備完了
	ClientReady,

    // <ホスト/クライアント>プレイヤー：自分の座標/角度
    PlayerTrans,
    // <ホスト/クライアント>プレイヤー：アニメの種類
    PlayerAnimeType,
    // <ホスト/クライアント>プレイヤー：アニメーション再生ステップ
    PlayerAnimeStep,
	// <ホスト/クライアント>プレイヤー：自分の入力
	PlayerInput,
	// <ホスト>プレイヤー：HP
	PlayerHp,
    // <ホスト>プレイヤー：被ダメ
    PlayerDamage,
    // <クライアント>プレイヤー：状態
    PlayerState,
    // <クライアント>当たり判定情報送信構造体
    PlayerCollOperator,

    // <ホスト>ボス : 自分の座標/角度
    BossTrans,
    // <ホスト>ボス : イベント (ステート遷移など)
    BossInform,
    // <ホスト/クライアント>ボス : HPとクリティカル情報
    BossHit,

	Max
};

// 送信チャンネル
enum class MSG_DATA_CHANNEL {
	None = -1,  // 未設定

	Reliable,   // 確実に届ける
	Unreliable, // 速度重視

    Max
};

// 送信チャンネルごとの送信保証フラグ
static constexpr enet_uint8 CHANNEL_PACKET_FLAG[(int)MSG_DATA_CHANNEL::Max] = {
    ENET_PACKET_FLAG_RELIABLE,
    0,
};

// ID
enum class MSG_SENDER_ID { None = -1, P1, P2, P3, P4, Max };

// 接続情報構造体
struct ConnectInfo { ENetPeer* peer; MSG_SENDER_ID senderId; };

// 接続状況
struct ConnectStatus {

private:
    // 参加状況 / 接続状況 マスク
    unsigned char entryMask = 1;
    unsigned char aliveMask = 1;

public:

#pragma region 取得系
    // 参加している総人数（切断中も含む）
    int EntryCount(void) const {
        int count = 0;
        for (int i = 0; i < (int)MSG_SENDER_ID::Max; i++) { if (entryMask & (1 << i)) count++; }
        return count;
    }

    // 特定のIDが「参加」しているか（切断中も含む）
    bool IsEntry(MSG_SENDER_ID id) const {
        if (id <= MSG_SENDER_ID::None || MSG_SENDER_ID::Max <= id) { return false; }
        return (entryMask & (1 << (int)id)) != 0;
    }

    // 特定のIDが「現在生存」しているか
    bool IsAlive(MSG_SENDER_ID id) const {
        if (id <= MSG_SENDER_ID::None || MSG_SENDER_ID::Max <= id) { return false; }
        return (aliveMask & (1 << (int)id)) != 0;
    }
#pragma endregion

#pragma region 設定系
    // メンバーを追加（参加と生存を同時にON）
    void AddMember(MSG_SENDER_ID id) {
        if (id <= MSG_SENDER_ID::None || MSG_SENDER_ID::Max <= id) { return; }
        unsigned char bit = (1 << (int)id);
        entryMask |= bit;
        aliveMask |= bit;
    }
    // メンバーを末尾に追加（参加と生存を同時にON）
    MSG_SENDER_ID AddMember(void) {
        // P1から順番にチェックして、entryMaskが立っていない（空いている）最初の枠を探す
        for (int i = 0; i < (int)MSG_SENDER_ID::Max; i++) {
            if (!IsEntry((MSG_SENDER_ID)i)) {
                AddMember((MSG_SENDER_ID)i);
                return (MSG_SENDER_ID)i;
            }
        }
        // 空きがない場合
        return MSG_SENDER_ID::None;
    }

    // 末尾のメンバーを完全に削除（最初からいなかったことにする）
    MSG_SENDER_ID RemoveMember(void) {
        // 後ろ（Max-1）から逆順に探す
        for (int i = (int)MSG_SENDER_ID::Max - 1; i >= 0; i--) {
            if (IsEntry((MSG_SENDER_ID)i)) {
                entryMask &= ~(1 << i);
                aliveMask &= ~(1 << i);

                return (MSG_SENDER_ID)i;
            }
        }
        return MSG_SENDER_ID::None;
    }

    // 通信切断のみ（参加状態は維持、生存だけOFF）
    void LostMember(MSG_SENDER_ID id) {
        if (id <= MSG_SENDER_ID::None || MSG_SENDER_ID::Max <= id) { return; }

        aliveMask &= ~(1 << (int)id);
    }

    // <復帰>「参加状態がON」「生存状態がOFF」の枠を探して、あれば復帰させる（どのIDとして復帰したかを返す）
    MSG_SENDER_ID RecoverMember(void) {
		// 参加状態がONで、生存状態がOFFの最初の枠を探す（P1から順番に）
        for (int i = 0; i < (int)MSG_SENDER_ID::Max; i++) {
            // 参加状態を見る
            if (!IsEntry((MSG_SENDER_ID)i)) { continue; }
            // 生存状態を見る
            if (IsAlive((MSG_SENDER_ID)i)) { continue; }

            // 条件を満たす最初の枠が見つかったら、そのIDを復帰させる
            AddMember((MSG_SENDER_ID)i);
            // 復帰させたIDを返す
            return (MSG_SENDER_ID)i;
        }
		// 条件を満たす枠がない場合
        return MSG_SENDER_ID::None;
    }

    // リセット（ホストだけの状態にする）
    void Reset(void) { entryMask = aliveMask = 1; }
#pragma endregion
};

#pragma region 各送信構造体定義

// ヘッダー（全ての構造体の先頭に配置する）
struct MsgDataHeader
{
    MSG_DATA_TYPE dataType;
    MSG_SENDER_ID senderId;

    MsgDataHeader(MSG_DATA_TYPE dataType) :
        dataType(dataType),
        senderId(MSG_SENDER_ID::None)
    {
    }
};

// <ホスト/クライアント>接続に関するシステムイベント送信構造体
struct MsgDataConnectInform
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::ConnectInform;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Reliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;

    // 接続イベント列挙型定義
    enum class INFORM_TYPE
    {
        None = -1,
        CloseReceptionToConnected,      // <ホスト>接続待ち終了（接続人数確定）
    };

	// 接続イベントの内容
    INFORM_TYPE inform;

    MsgDataConnectInform(INFORM_TYPE inform) :
        header(DATA_TYPE),
        inform(inform)
    {
    }
    MsgDataConnectInform(void) :
        header(DATA_TYPE),
        inform(INFORM_TYPE::None)
    {
    }
};

// <ホスト>ID送信構造体
struct MsgDataSenderId
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::SenderId;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Reliable;

    MsgDataHeader header;
    MSG_SENDER_ID senderId;

    MsgDataSenderId(MSG_SENDER_ID senderId) :
        header(DATA_TYPE),
        senderId(senderId)
    {
    }
    MsgDataSenderId(void) :
        header(DATA_TYPE),
        senderId(MSG_SENDER_ID::None)
    {
    }
};

// <ホスト>接続状況送信構造体
struct MsgDataConnectStatus
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::ConnectStatus;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Reliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;

    ConnectStatus connectStatus;

    MsgDataConnectStatus(ConnectStatus connectStatus) :
        header(DATA_TYPE),
        connectStatus(connectStatus)
    {
    }
    MsgDataConnectStatus(void) :
        header(DATA_TYPE),
        connectStatus()
    {
    }
};

// <ホスト>システムイベント送信構造体
struct MsgDataSystemInform
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::SystemInform;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Reliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;

    // システムイベント列挙型定義
    enum class INFORM_TYPE
    {
        None = -1,

		// 接続完了（ホストが接続待ちを終了して、接続人数が確定した）
		ConnectComplete,

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

    INFORM_TYPE inform;

    MsgDataSystemInform(INFORM_TYPE inform) :
        header(DATA_TYPE),
        inform(inform)
    {
    }
    MsgDataSystemInform(void) :
        header(DATA_TYPE),
        inform(INFORM_TYPE::None)
    {
    }
};

// <ホスト/クライアント>キャラセレクト送信構造体
struct MsgDataCharaSelect
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::CharaSelect;
    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Unreliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;

	// キャラタイプ
    int charaType;

    MsgDataCharaSelect(int charaType) :
        header(DATA_TYPE),
        charaType(charaType)
    {
    }
    MsgDataCharaSelect(void) :
        header(DATA_TYPE),
        charaType()
    {
    }
};

// <クライアント>ロビーでの準備完了送信構造体
struct MsgDataClientReady
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::ClientReady;
    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Unreliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;

	// 準備完了フラグ
    bool ready;

    MsgDataClientReady(bool ready) :
        header(DATA_TYPE),
        ready(ready)
    {
    }
    MsgDataClientReady(void) :
        header(DATA_TYPE),
        ready()
    {
    }
};

// <ホスト/クライアント>プレイヤー移動情報送信構造体
struct MsgDataPlayerTrans
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::PlayerTrans;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Unreliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;
    Vector3 pos;
    Vector3 angle;

    MsgDataPlayerTrans(const Vector3& pos, const Vector3& angle) :
        header(DATA_TYPE),
        pos(pos),
        angle(angle)
    {
    }
    MsgDataPlayerTrans(void) :
        header(DATA_TYPE),
        pos(),
        angle()
    {
    }
};

struct MsgDataPlayerAnimeType
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::PlayerAnimeType;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Unreliable;

    // ヘッダー
    MsgDataHeader header;

    int animeType;
    bool loop;

    MsgDataPlayerAnimeType(int animeType, bool loop) :
        header(DATA_TYPE),
        animeType(animeType),
        loop(loop)
    {
    }
    MsgDataPlayerAnimeType() :
        header(DATA_TYPE),
        animeType(),
        loop()
    {
    }
};

// <ホスト/クライアント>プレイヤー入力送信構造体
struct MsgDataPlayerInput
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::PlayerInput;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Unreliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;
    KEY_TYPE type;
    Key::KEY_INFO key;

    MsgDataPlayerInput(KEY_TYPE type, const Key::KEY_INFO& key) :
        header(DATA_TYPE),
        type(type),
        key(key)
    {
    }
    MsgDataPlayerInput(void) :
        header(DATA_TYPE),
        type(),
        key()
    {
    }
};

// <ホスト/クライアント>プレイヤー入力送信構造体
struct MsgDataPlayerDamage
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::PlayerDamage;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Unreliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;

    // ダメージ量
    short damage;

    MsgDataPlayerDamage(short damage) :
        header(DATA_TYPE),
        damage(damage)
    {
    }
    MsgDataPlayerDamage(void) :
        header(DATA_TYPE),
        damage()
    {
    }
};

// <ホスト>被ダメ送信構造体
struct MsgDataPlayerHp
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::PlayerHp;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Unreliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;
    int hp;

    MsgDataPlayerHp(int hp) :
        header(DATA_TYPE),
        hp(hp)
    {
    }
    MsgDataPlayerHp(void) :
        header(DATA_TYPE),
        hp()
    {
    }
};

// <ホスト>被ダメ送信構造体
struct MsgDataPlayerState
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::PlayerState;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Unreliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;
    int state;

    MsgDataPlayerState(int state) :
        header(DATA_TYPE),
        state(state)
    {
    }
    MsgDataPlayerState(void) :
        header(DATA_TYPE),
        state()
    {
    }
};

// <ホスト>当たり判定情報送信構造体
struct MsgDataPlayerCollOperator
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::PlayerCollOperator;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Unreliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;

    // 当たり判定の種類の列挙型定義
    enum class COLLIDER_KINDS
    {
        Non = -1,

        CommonPlayerTripleAttack_1,
        CommonPlayerTripleAttack_2,
        CommonPlayerTripleAttack_3,
        CommonPlayerSimpleAttack,
        
        Max
    };
    // 当たり判定の種類の列挙型定義の変数定義
    COLLIDER_KINDS collKinds;

    // 当たり判定：ON / OFF
    bool isCollider;

    MsgDataPlayerCollOperator(bool isCollider, COLLIDER_KINDS collKinds) :
        header(DATA_TYPE),
        isCollider(isCollider),
        collKinds(collKinds)
    {
    }
    MsgDataPlayerCollOperator(void) :
        header(DATA_TYPE),
        isCollider(),
        collKinds()
    {
    }
};

// <ホスト>ボス移動情報送信構造体
struct MsgDataBossTrans
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::BossTrans;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Unreliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;
    Vector3 pos;
    Vector3 angle;

    MsgDataBossTrans(const Vector3& pos, const Vector3& angle) :
        header(DATA_TYPE),
        pos(pos),
        angle(angle)
    {
    }
    MsgDataBossTrans(void) :
        header(DATA_TYPE),
        pos(),
        angle()
    {
    }
};

// <ホスト>ボスイベント送信構造体
struct MsgDataBossInform
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::BossInform;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Reliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;

    // システムイベント列挙型定義
    enum class INFORM_TYPE
    {
        None = -1,

        // アイドルステートへ遷移
        ChangeIdle,
        // ムーブステートへ遷移
        ChangeMove,
        
        // 攻撃Aステートへ遷移
        ChangeAttackA,
        // 攻撃Bステートへ遷移
        ChangeAttackB,
        // 攻撃Cステートへ遷移
        ChangeAttackC,
        // 攻撃Dステートへ遷移
        ChangeAttackD,
        // 攻撃Eステートへ遷移
        ChangeAttackE,
    };

    INFORM_TYPE inform;

    MsgDataBossInform(INFORM_TYPE inform) :
        header(DATA_TYPE),
        inform(inform)
    {
    }
    MsgDataBossInform(void) :
        header(DATA_TYPE),
        inform(INFORM_TYPE::None)
    {
    }
};

// <ホスト>ボス移動情報送信構造体
struct MsgDataBossHit
{
    // 列挙型定義との紐づけ
    static constexpr MSG_DATA_TYPE DATA_TYPE = MSG_DATA_TYPE::BossHit;

    // データの送信チャンネル
    static constexpr MSG_DATA_CHANNEL DATA_CHANNEL = MSG_DATA_CHANNEL::Unreliable;

    // ヘッダー（全ての構造体の先頭に配置する）
    MsgDataHeader header;
    int damage;
    bool clitical;

    MsgDataBossHit(const int& damage, const bool& clitical) :
        header(DATA_TYPE),
        damage(damage),
        clitical(clitical)
    {
    }
    MsgDataBossHit(void) :
        header(DATA_TYPE),
        damage(),
        clitical()
    {
    }
};

#pragma endregion