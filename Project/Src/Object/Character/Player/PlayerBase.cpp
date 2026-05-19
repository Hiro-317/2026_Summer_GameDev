#include "PlayerBase.h"

#include "../../../Manager/Camera/Camera.h"
#include "../../../Manager/Font/FontManager.h"

#include "CommonPlayerState/Move/PlayerMoveState.h"

#include "../../Common/Collider/LineCollider.h"
#include "../../Common/Collider/CapsuleCollider.h"

PlayerBase::PlayerBase(
	short HP_MAX,
	short ATTACK_POWER,
	short DEFENSE_POWER,
	short SPEED_POWER,

	std::string modelPath
) :
	CharacterBase(
		HP_MAX,
		ATTACK_POWER,
		DEFENSE_POWER,
		SPEED_POWER)
{
	trans.Load(("Charactor/" + modelPath).c_str());
}

PlayerBase::PlayerBase(
	short HP_MAX,
	short ATTACK_POWER, 
	short DEFENSE_POWER,
	short SPEED_POWER, 
	const std::string& parameterPath,

	std::string modelPath
) :
	CharacterBase(
		HP_MAX,
		ATTACK_POWER,
		DEFENSE_POWER,
		SPEED_POWER,
		parameterPath)
{
	trans.Load(("Charactor/" + modelPath).c_str());
}

void PlayerBase::CharacterLoad(void)
{
	PlayerLoad();

#pragma region モデル
	// モデルを読み込む

	// モデルのスケールを設定
	trans.scale = MODEL_SCALE;

	// モデルの中心点のズレ
	trans.centerDiff = MODEL_CENTER_DIFF;

	// 角度のズレ(ローカル回転)
	trans.localAngle = MODEL_LOCAL_ROT;

#pragma endregion

#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 衝突時の押し出しを有効にする
	SetPushFlg(true);

	// 押し出す力の大きさを設定する
	SetPushWeight(COLLISION_PUSH_WEIGHT);

	// 無敵中に赤点滅を表示しない
	SetInviEffectFlg(false);

#pragma endregion

#pragma region 当たり判定情報設定

	// 当たり判定を生成する（線分コライダー）
	ColliderCreate(new LineCollider(COLLIDER_TAG::PLAYER, LINE_COLLIDER_START_POS, LINE_COLLIDER_END_POS));

	// 当たり判定を生成する（カプセルコライダー）
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::PLAYER,
			CAPSULE_COLLIDER_START_POS, CAPSULE_COLLIDER_END_POS,
			CAPSULE_COLLIDER_RADIUS,
			CAPSULE_COLLIDER_ENOUGH_DISTANCE
		)
	);

#pragma endregion

	// まとめて読み込み処理
	for (ActorBase*& c : subObjArray) { c->Load(); }
}

void PlayerBase::CharactorInit(void)
{
	// 位置を初期位置にする
	trans.pos = INIT_POS;

	// 初期状態を移動状態にする
	state = (int)STATE::MOVE;

	for (ActorBase*& c : subObjArray) { c->Init(); }
}

void PlayerBase::CharactorUpdate(void)
{
	for (ActorBase*& c : subObjArray) { c->Update(); }

	interestPos = trans.pos + INTEREST_POS;

	if (characterStats.hp <= 0) {
		//ChangeState((int)STATE::DEATH);
	}

#ifdef _DEBUG		// クールタイム用
	if (state == (int)STATE::DEATH) {
		// 不動オブジェクトにする
		SetDynamicFlg(false);

		// カメラを固定する
		Camera::GetIns().ChangeModeFixedPoint(trans.pos + Vector3::YZonly(250, -550), Deg2Rad(30));

		// 死亡状態に遷移する
		ChangeState((int)STATE::DEATH);
	}
#endif // _DEBUG

	PlayerUpdate();
}

void PlayerBase::CharactorDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->Draw(); }
}

void PlayerBase::CharactorAlphaDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->AlphaDraw(); }
}

void PlayerBase::CharacterUiDraw(void)
{
	if (App::GetIns().IsDrawDebug()) {

		// 1行ずつ描画するためのラムダ式（デバッグ用）
		int yPos = 150; const int FONT_SIZE = 20;
		auto debugDrwStr = [&](std::string str)->void {
			DrawStringToHandle(0, yPos, str.c_str(), 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_20));
			yPos += FONT_SIZE;
			};

		// 加速度をデバッグ表示
		debugDrwStr("プレイヤー～～～～～～");
		debugDrwStr("座標" + std::to_string(trans.pos.x) + ", " + std::to_string(trans.pos.y) + ", " + std::to_string(trans.pos.z));
		debugDrwStr("加速度:" + std::to_string(accelSum.Length()));
		debugDrwStr("スタミナ:" + std::to_string(dynamic_cast<PlayerMoveState&>(GetStateIns((int)STATE::MOVE)).GetDashStamina()));
		debugDrwStr("息切れ:" + std::string(dynamic_cast<PlayerMoveState&>(GetStateIns((int)STATE::MOVE)).IsTired() ? "true" : "false"));
		debugDrwStr("～～～～～～('#；ω;`)");
	}
}

void PlayerBase::OnCollision(const ColliderBase& collider)
{
	if (GetInviCounter() > 0) { return; }

	// 回避中の無敵処理
	if (state == (int)STATE::SKILL_3) {
		switch (collider.GetTag()) {
		case COLLIDER_TAG::BOSS_ATTACK_1:
			SetInviCounter(150);
			// ミスの表示
			break;
		}
		return;
	}

	if (state == (int)STATE::DEATH) { return; }

	switch (collider.GetTag()) {
	case COLLIDER_TAG::BOSS_ATTACK_1:
		characterStats.hp -= CalculateDamage(collider.GetSkillStats().Power(), characterStats.defensePower.Value());
		ChangeState((int)STATE::DAMAGE);
		break;
	}

}

void PlayerBase::CharactorRelease(void)
{
	for (ActorBase*& c : subObjArray) {
		if (c) {
			c->Release();
			delete c;
			c = nullptr;
		}
	}
	subObjArray.clear();
}