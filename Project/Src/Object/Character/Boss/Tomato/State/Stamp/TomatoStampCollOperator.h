#pragma once

#include "../../../../../ActorBase.h"

class TomatoStampCollOperator : public ActorBase
{
public:

	/// <summary>
	/// コンストラクト
	/// </summary>
	/// <param name="ATTACK_RADIUS">攻撃範囲</param>
	/// <param name="ATTACK_DURATION">攻撃判定の持続時間</param>
	TomatoStampCollOperator(
		float ATTACK_RADIUS,
		const bool& isGournd,
		const std::vector<const Vector3*> playerPos,
		const MSG_SENDER_ID operatorSenderId,
		const CharacterStats& stats,
		const ParameterLoad& collParam
	);
	~TomatoStampCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	// 攻撃の中心座標の取得
	Vector3 GetAttackPos(void) { return collBack.pos; }

	void SetScale(float scale) { collFront.scale = SCALE * scale; }

	// 攻撃範囲の描画設定
	void SetDrawArea(bool flg) { isDrawArea = flg; }
	void CollSet(int num, bool flg) { ColliderSerch(COLLIDER_TAG::BOSS_ATTACK).at(num)->SetJudgeFlg(flg); }

	// 攻撃終了
	bool End(void) { return end; }

private:

#pragma region 定数

	// 攻撃範囲の半径
	const float ATTACK_RADIUS;

	// コライダー描画の高さ
	const float HEIGHT;

	// コライダー描画の拡大率
	const Vector3 SCALE;

#pragma endregion

#pragma region 受け取る参照変数

	const bool& ground;

	const std::vector<const Vector3*> playerPos;

	const MSG_SENDER_ID operatorSenderId;

	const CharacterStats& stats;

#pragma endregion

#pragma region 変数

	// 攻撃終了
	bool end;

	// 攻撃カウント
	int attackCnt;

	// 攻撃範囲描画フラグ
	bool isDrawArea;

	Transform collBack;
	Transform collFront;

#pragma endregion

	// 更新処理
	void SubUpdate(void)override;

	void SubAlphaDraw(void)override;
};