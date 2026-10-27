#pragma once

#include "../../PlayerBase.h"

#include "../../../../../Common/Vector2.h"

class PlayerTripleAttackCollOperator;
class PlayerSimpleAttackCollOperator;

class HitUI;

class OrangePlayer : public PlayerBase
{
public:
	OrangePlayer(MSG_SENDER_ID controlSenderId);
	~OrangePlayer()override = default;

	void PlayerLoad(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;

private:

#pragma region 定数定義

	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～

	// アニメーションの種類
	enum class ANIME_TYPE
	{
		NONE = -1,

		IDLE,
		WALK,
		RUN,
		PUNCH1,
		PUNCH2,
		PUNCH3,
		KICK,
		DODGE,
		DAMAGE,
		DEATH,

		MAX
	};

	// アニメーションのタイプごとの再生速度
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::MAX] = 
	{
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "WalkAnimeSpeed"),	// WALK
		GetParameter("Anime", "RunAnimeSpeed"),		// RUN
		GetParameter("Anime", "Punch1AnimeSpeed"),	// PUNCH1
		GetParameter("Anime", "Punch2AnimeSpeed"),	// PUNCH2
		GetParameter("Anime", "Punch3AnimeSpeed"),	// PUNCH3
		GetParameter("Anime", "KickAnimeSpeed"),	// KICK
		GetParameter("Anime", "DodgeAnimeSpeed"),	// DODGE
		GetParameter("Anime", "DamageAnimeSpeed"),	// DAMAGE
		GetParameter("Anime", "DeathAnimeSpeed"),	// DEATH
	};

	// アニメーションのフォルダまでのパス
	const std::string ANIME_FOLDER_PATH = "Data/Model/Character/Orange/Anime/";

	// アニメーションのタイプごとのパス
	const std::string ANIME_PATH_TABLE[(int)ANIME_TYPE::MAX] =
	{
		ANIME_FOLDER_PATH + "Idle.mv1",		// IDLE
		ANIME_FOLDER_PATH + "Walk.mv1",		// WALK
		ANIME_FOLDER_PATH + "Run.mv1",		// RUN
		ANIME_FOLDER_PATH + "Punch1.mv1",	// PUNCH1
		ANIME_FOLDER_PATH + "Punch2.mv1",	// PUNCH2
		ANIME_FOLDER_PATH + "Punch3.mv1",	// PUNCH3
		ANIME_FOLDER_PATH + "Kick.mv1",		// KICK
		ANIME_FOLDER_PATH + "Dodge.mv1",	// DODGE
		ANIME_FOLDER_PATH + "Hit.mv1",		// DAMAGE
		ANIME_FOLDER_PATH + "Death.mv1"		// DEATH
	};

	// アニメーションのタイプごとのループ再生フラグ
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::MAX] =
	{
		true, // IDLE
		true, // WALK
		true, // RUN
		false, // PUNCH1
		false, // PUNCH2
		false, // PUNCH3
		false, // KICK
		false, // DODGE
		false, // DAMAGE
		false, // DEATH
	};

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～アニメーション

#pragma endregion 定数定義

	// 三段攻撃の当たり判定管理クラスの参照
	PlayerTripleAttackCollOperator* tripleAttackCollOperatorRef;

	// 蹴り攻撃の当たり判定管理クラスの参照
	PlayerSimpleAttackCollOperator* kickCollOperatorRef;

	// 回避中の無敵判定
	bool isInvincible;

	// 回避に使うヒットUIクラスの参照
	HitUI* hitUiRef;

	void SubDraw(void)override {
		SetFontSize(32);
		DrawString(0, 300, isInvincible ? "回避中！" : "回避してない！", 0xffffff);
		SetFontSize(16);
	}
};