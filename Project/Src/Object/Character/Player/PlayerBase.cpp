#include "PlayerBase.h"

#include "CommonPlayerState/TripleAttack/PlayerTripleAttackStDefine.h"



{
}

PlayerBase::PlayerBase(
	short HP_MAX,
	short ATTACK_POWER,
	short DEFENSE_POWER,
	short SPEED_POWER, 
	
	std::string modelPath, 
	Vector3 SCALE, 
	Vector3 MODEL_CENTER_DIFF, 
	Vector3 MODEL_LOCAL_ROT
) :
	CharacterBase(
		HP_MAX,
		ATTACK_POWER,
		DEFENSE_POWER,
		SPEED_POWER)
{
}

PlayerBase::PlayerBase(
	short HP_MAX,
	short ATTACK_POWER, 
	short DEFENSE_POWER,
	short SPEED_POWER, 
	const std::string& parameterPath, 

	std::string modelPath,
	Vector3 SCALE,
	Vector3 MODEL_CENTER_DIFF,
	Vector3 MODEL_LOCAL_ROT
) :
	CharacterBase(
		HP_MAX,
		ATTACK_POWER,
		DEFENSE_POWER,
		SPEED_POWER,
		parameterPath)
{
}

void PlayerBase::CharacterLoad(void)
{
}

void PlayerBase::CharacterUiDraw(void)
{
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