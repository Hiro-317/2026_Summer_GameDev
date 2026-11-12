#pragma once

#include "../../../../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../../../../Common/Vector3.h"

class GrapePlayerWeaponThrow;

struct Transform;

class GrapePlayerThrowState : public CharacterStateBase
{
public:

	// コンストラクタ
	GrapePlayerThrowState(
		const int COOL_TIME,

		GrapePlayerWeaponThrow& throwWeapon,

		Transform& playerTrans,

		const Vector3*& bossPos,

		const std::function<void(void)>& playAnime,
		const std::function<float(void)>& getAnimeRatio,
		const std::function<void(void)>& defaultChangeState
	);

	// デストラクタ
	~GrapePlayerThrowState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	void Update(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

	// クールタイムを参照
	const float& GetCoolTimeCounter(void)const { return coolTimeCounter; }

private:
#pragma region 定数定義

	// クールタイム
	const int COOL_TIME;

#pragma endregion 

#pragma region 変数定義

	GrapePlayerWeaponThrow& throwWeapon;

	// モデル制御情報の参照
	Transform& playerTrans;

	const Vector3*& bossPos;

	// 終了までのカウント用
	float timeCounter;

	const std::function<void(void)> playAnime;
	const std::function<float(void)> getAnimeRatio;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> defaultChangeState;

#pragma endregion 

	Vector3 moveDir;

	bool isThrow;

	float coolTimeCounter;
};