#pragma once

#include "../../Common/CharacterBase/CharacterBase.h"

#include "../../../Application/Application.h"

#include "../../../Manager/Net/SenderID_Define.h"

#include "CommonPlayerState/TripleAttack/PlayerTripleAttackStDefine.h"

class PlayerBase : public CharacterBase
{
public:

	/// <summary>
	/// プレイヤーベースのコンストラクタ
	/// </summary>
	/// <param name="controlSenderId">誰が操作権を持つか</param>
	/// <param name="fileName">パラメーターのパス</param>
	/// <param name="hpParameterID">HPパラメーターの名前</param>
	/// <param name="attackPowerParameterID">攻撃力パラメーターの名前</param>
	/// <param name="defensePowerParameterID">防御力パラメーターの名前</param>
	/// <param name="moveSpeedParameterID">移動速度パラメーターの名前</param>
	/// <param name="parameterPath">外部からパラメーターを読み込むためのファイルパス</param>
	/// <param name="modelPath">プレイヤーモデルのファイルパス</param>
	PlayerBase(
		MSG_SENDER_ID controlSenderId,

		const std::string fileName,
		const std::string hpParameterID,
		const std::string attackPowerParameterID,
		const std::string defensePowerParameterID,
		const std::string moveSpeedParameterID,

		const std::string& parameterPath,

		std::string modelPath
	);

	~PlayerBase()override = default;

	void Load(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	virtual void ReceptionUpdate(void)override;
	virtual void SendUpdate(void)override;

	// ボスの座標取得
	void SetBossPos(const Vector3* bossPos) { this->bossPos = bossPos; }

	// 注視点
	const Vector3& GetInterestPos(void) const { return INTEREST_POS; }

	// 自身以外のプレイヤー座標を取得
	void SetOtherPlayerTrans(const Transform* pos) { otherPlayerTrans.emplace_back(pos); }

	bool GetIsDeath(void)const { return false; }

private:

	// メイン処理～～～～～～～～～～～～～～
	void SubInit(void)override;
	void SubUpdate(void)override;
	// ～～～～～～～～～～～～～～メイン処理
	
	// 個別ロード
	virtual void PlayerLoad(void) = 0;

protected:


#pragma region 定数定義

	// モデルサイズ
	const Vector3 MODEL_SIZE = GetParameterToVector3("Model", "Size") * GetParameterToVector3("Model", "Scale");

	// 注視点の相対座標
	const Vector3 INTEREST_POS = GetParameterToVector3("Init", "InterestPos");

	// 状態の種類
	enum class STATE
	{
		NONE = -1,

		IDLE,			// 待機状態

		MOVE,			// 移動状態

		SKILL_1,		// スキル1（基本的に「通常攻撃」）
		SKILL_2,		// スキル2（キャラごとの「特殊技」）
		SKILL_3,		// スキル3（キャラごとの「特殊技」）
		SPECIAL_SKILL,	// スペシャルスキル（キャラごとの「必殺技」）

		DAMAGE,			// ダメージ
		DEATH,			// 倒される
		OTHER_WATCH,

		MAX
	};

#pragma region UI

	// プレイヤーUI画像のファイルパス
	static const inline std::string PLAYER_UI_IMAGE_FILE_PATH = "Data/Image/Game/UI/Player/";

	// <スキル1>UI描画位置
	static const inline Vector2I SKILL1_UI_DRAW_POS = Vector2I(App::SCREEN_SIZE_X_HALF + 600, App::SCREEN_SIZE_Y_HALF + 300);
	// <スキル2>UI描画位置
	static const inline Vector2I SKILL2_UI_DRAW_POS = Vector2I(SKILL1_UI_DRAW_POS.x - 120, SKILL1_UI_DRAW_POS.y - 100);
	// <スキル3>UI描画位置
	static const inline Vector2I SKILL3_UI_DRAW_POS = Vector2I(SKILL1_UI_DRAW_POS.x - 240, SKILL1_UI_DRAW_POS.y);

	// 画像の名前（HPフレーム）
	static const inline std::string HP_FRAME_IMAGE_NAME = "HP/PlayerHpFrame.png";
	// 画像の名前（HPバー緑）
	static const inline std::string HP_IMAGE_NAME = "HP/PlayerHp.png";
	// 画像の名前（HPバー赤）
	static const inline std::string HP_LOST_IMAGE_NAME = "HP/PlayerHpLost.png";

	// 画像サイズ（HPフレーム）
	static const inline Vector2I HP_FRAME_IMAGE_SIZE = Vector2I(300, 40);
	// 画像サイズ（HPバー）
	static const inline Vector2I HP_BAR_IMAGE_SIZE = Vector2I(283, 40);

	// 画像の名前（アイコン背景）
	static const inline std::string ICON_BACK_IMAGE_NAME[(int)MSG_SENDER_ID::Max] = {
		"HP/P1Back.png",
		"HP/P2Back.png",
		"HP/P3Back.png",
		"HP/P4Back.png",
	};
	// 画像の名前（アイコン枠）
	static const inline std::string ICON_FRAME_IMAGE_NAME[(int)MSG_SENDER_ID::Max] = {
		"HP/P1Frame.png",
		"HP/P2Frame.png",
		"HP/P3Frame.png",
		"HP/P4Frame.png",
	};

	// HPバー（アイコン付き）の描画位置テーブル
	const Vector2I HP_UI_DRAW_POS[(int)MSG_SENDER_ID::Max] = {
		Vector2I(80, App::SCREEN_SIZE_Y_HALF),
		Vector2I(80, App::SCREEN_SIZE_Y_HALF + (HP_IMAGE_SIZE.y + 35)),
		Vector2I(80, App::SCREEN_SIZE_Y_HALF + ((HP_IMAGE_SIZE.y + 35) * 2)),
		Vector2I(80, App::SCREEN_SIZE_Y_HALF + ((HP_IMAGE_SIZE.y + 35) * 3)),
	};
#pragma endregion

	// 自身以外のプレイヤーのモデル制御情報
	std::vector<const Transform*>otherPlayerTrans;

	// ボスの座標のポインタ
	const Vector3* bossPos;

	// 味方プレイヤー内のターゲット
	unsigned char targetPlayerIndex;

	// 味方プレイヤー内のターゲット参照座標
	const Vector3* targetPlayerPos;

	// 味方プレイヤーのターゲットを次へ
	void TargetPlayerNext(void);

	// ステート遷移関数
	void ChangeState(int nextState)override;
	using CharacterBase::ChangeState;

	// モデルアニメーションプレイ関数
	void AnimePlay(int type, signed char loop = -1)override;
	using CharacterBase::AnimePlay;
};
