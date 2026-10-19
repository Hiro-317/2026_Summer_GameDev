#pragma once

#include "../../PlayerBase.h"

#include "../../../../../Common/Vector2.h"

class PlayerSimpleAttackCollOperator;
class PlayerSingleModifierCollOperator;

class PeachPlayer : public PlayerBase
{
public:

	// コンストラクタ
	PeachPlayer(MSG_SENDER_ID operatorSenderId);
	~PeachPlayer()override = default;

	// ロード
	void PlayerLoad(void)override;

	// 同期処理
	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;

private:

	// アニメーションの種類
	enum class ANIME_TYPE
	{
		NONE = -1,

		IDLE,
		WALK,
		RUN,
		DAMAGE,
		DEATH,
		PUNCH,
		HEAL,

		MAX
	};

	// アニメーションの再生速度
	const float ANIME_SPEED[(int)ANIME_TYPE::MAX] = {
		1.0f,	// IDLE,
		1.0f,	// WALK,
		1.0f,	// RUN,
		1.0f,	// DAMAGE,
		1.0f,	// DEATH,
		1.0f,	// PUNCH,
		2.5f	// HEAL,
	};

	// 通常攻撃(パンチ)の当たり判定管理クラスの参照
	PlayerSimpleAttackCollOperator* punchCollOperatorRef;

	// 回復の当たり判定管理クラスの参照
	PlayerSingleModifierCollOperator* healCollOperatorRef;

	// バフの当たり判定管理クラスの参照
	PlayerSingleModifierCollOperator* buffCollOperatorRef;
};