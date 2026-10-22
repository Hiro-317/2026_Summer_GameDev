#include "TomatoPlayerTackleState.h"

#include "../../../../../../../Manager/Input/InputManager.h"
#include "../../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../../Manager/Camera/CurrentCamera.h"

#include "../../../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackCollOperator.h"

TomatoPlayerTackleState::TomatoPlayerTackleState(

	PlayerSimpleAttackCollOperator& collOperator,

	Transform& playerTrans,
	
	const std::function<void(void)> chargeAttackBuff,
	const std::function<void(void)> chargeAttackBuffDelete,
	const std::function<void(void)> defaultChangeState
) :
	collOperator(collOperator),

	playerTrans(playerTrans),

	chargeAttackBuff(chargeAttackBuff),
	chargeAttackBuffDelete(chargeAttackBuffDelete),
	defaultChangeState(defaultChangeState)
{

}

void TomatoPlayerTackleState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0.0f) { return; }

	// ステート遷移処理
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerSkill2).down) {
		OwnChangeState();
	}
}

void TomatoPlayerTackleState::Enter(void)
{
	//	当たったかどうかのフラグをリセット
	collOperator.ResetIsHit();

	// 突進方向の取得
	moveDir.x = sinf(playerTrans.rotation.ToEulerXZY().y);
	moveDir.z = cosf(playerTrans.rotation.ToEulerXZY().y);
	moveDir.Normalize();

	// X軸の角度をゼロに初期化(回転量で突進の開始を判断しているから、初期化しておく)
	playerTrans.rotation = Quaternion::FromRotationY(atan2f(moveDir.x, moveDir.z));

	// カウンターの初期化
	timeCounter = COUNT_MAX;
	coolTimeCounter = chargeCounter;

	step = STEP::Charge;
}

void TomatoPlayerTackleState::Update(void)
{
	// スキルUIのチャージがcoolTimeCounterを参照して動くため同期しておく
	coolTimeCounter = chargeCounter;

	switch (step) {

	case TomatoPlayerTackleState::STEP::Charge: {

		// これはスキルのUIをチャージ用に動かすための特殊処理処理
		Charge(chargeCounter, COOL_TIME, CHARGE_TIME);

		// 回転スタート
		playerTrans.rotation.AddAngle(Vector3(ROTATION_POW, 0.0f, 0.0f));

		// チャージの効果音を再生
		SoundManager::GetIns().Play("TackleCharge");

		// 入力を取得
		moveDir = InputVec().Normalized();

		// 移動方向をカメラで回転させる
		moveDir.TransMatOwn(MGetRotY(CurrentCamera::Get().GetRotation().GetYaw()));

		// 角度を入力方向に向ける
		if (moveDir != 0.0f) { playerTrans.rotation = Quaternion::FromRotationY(atan2f(moveDir.x, moveDir.z)); }

		// 離した瞬間にタックル開始
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerSkill2).up) {
			step = STEP::Tackle;

			moveDir = Vector3(sinf(playerTrans.rotation.ToEulerXZY().y), 0.0f, cosf(playerTrans.rotation.ToEulerXZY().y));
			moveDir.Normalize();
		}

		break;
	}

	case TomatoPlayerTackleState::STEP::Tackle: {
		Tackle();
		break;
	}
		break;
	}
}

void TomatoPlayerTackleState::Exit(void)
{
	// 念のためすべて初期化
	timeCounter = 0;
	collOperator.CollOff();
	collOperator.ResetIsHit();

	// スキルのクールタイムをセット
	coolTimeCounter = COOL_TIME;
	chargeCounter = 0;

	// チャージ攻撃のバフを削除
	chargeAttackBuffDelete();
}

void TomatoPlayerTackleState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}

void TomatoPlayerTackleState::Tackle(void)
{
	// チャージの効果音を消して、タックルの効果音を再生
	SoundManager::GetIns().Stop("TackleCharge");
	SoundManager::GetIns().Play("TackleMove");

	// 突進中も回転を続ける
	playerTrans.rotation.AddAngle(Vector3(ROTATION_POW, 0.0f, 0.0f));
	// カウントを開始
	timeCounter--;
	// 前方に向かって突進
	playerTrans.pos += moveDir * MOVE_SPEED;

	// チャージできていたら、バフをかけて攻撃力を増やす
	if (chargeCounter >= COOL_TIME) {
		chargeAttackBuff();
	}

	// 当たり判定をオンにする
	collOperator.CollOn();

	// 当たったらタイマーを強制的に終了し、次の処理へ
	if (collOperator.GetIsHit()) {
		timeCounter = 0;
		collOperator.CollOff();
	}
}

Vector3 TomatoPlayerTackleState::InputVec(void) const
{
	// 返却用一時変数
	Vector3 vec = Vector3();

	// コントローラーの入力を取得
	vec = Input::GetIns().GetLeftStickVec(false).ToVector3XZInvertY();

	// 入力がなければ次にキーボードの入力を取得
	if (vec == 0.0f) {
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
		vec.Normalize();
	}

	return vec;
}
