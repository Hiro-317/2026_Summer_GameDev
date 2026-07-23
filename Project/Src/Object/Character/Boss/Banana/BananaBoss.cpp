#include "BananaBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Font/FontManager.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"

#include "State/Idle/BananaBossIdleState.h"
//#include "State/KickDown/GrapeBossKickDownState.h"
//#include "State/KickDown/GrapeKickDownCollOperator.h"
//#include "State/Straight/GrapeBossStraightState.h"
//#include "State/Stamp/GrapeBossStampState.h"
//#include "State/Stamp/GrapeStampCollOperator.h"
//#include "State/Single/GrapeBossSingleState.h"
//#include "State/Stalker/GrapeBossStalkerState.h"
//#include "State/Random/GrapeBossRandomState.h"
#include "State/Death/BananaBossDeathState.h"

#include "../../../../Scene/Game/GameScene.h"

#include "../../../../Manager/Effect/EffectManager.h"

#include "../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../UI/HitUI/HitUI.h"


BananaBoss::BananaBoss(const std::vector<const Vector3*> playerPos, const std::vector<const bool*> playerLive) :
	BossBase(
		"HP",
		"AttackPower",
		"DefensePower",
		"MoveSpeed",
		"Data/Parameter/Character/Boss/Banana/",
		"Banana/Banagon",

		playerPos, playerLive)
{

	coolTime = 120;
}

void BananaBoss::PlayerLoad(void)
{
	// ƒAƒjƒ[ƒVƒ‡ƒ“````````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚ğ¶¬‚·‚é
	CreateAnimationController();

	// ŠeíƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED_TABLE);

	// ```````````````````````````````````

#pragma region “–‚½‚è”»’èî•ñİ’è

	// “–‚½‚è”»’è‚ğ¶¬‚·‚éiƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[j
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::BOSS_DISTANCE,
			CAPSULE_COLLIDER_START_POS, CAPSULE_COLLIDER_END_POS,
			CAPSULE_COLLIDER_RADIUS
		)
	);

#pragma endregion


#pragma region ƒvƒŒƒCƒ„[‚ª•ø‚¦‚é‰ºˆÊƒNƒ‰ƒX‚ğ¶¬‚·‚é

	//subObjArray.push_back(new GrapeKickDownCollOperator(operatorSenderId, characterStats));

	AddState(
		static_cast<int>(STATE::IDLE),
		new BananaBossIdleState(
			// ©•ª‚Ìó‘Ô‚É‘JˆÚ‚·‚éŠÖ”
			[&]() { state = static_cast<int>(STATE::IDLE); },
			// ©•ª‚Ìó‘Ô‚©‚Ç‚¤‚©‚ğ•Ô‚·ŠÖ”
			[&]() { return state == static_cast<int>(STATE::IDLE); },
			// ©•ª‚ÌÀ•WAƒvƒŒƒCƒ„[‚ÌÀ•W‚Ì“Ç‚İæ‚è
			trans.pos, trans.angle, playerPos,
			// ƒN[ƒ‹ƒ^ƒCƒ€
			[&]() { return coolTime; },
			// ƒ^[ƒQƒbƒg”Ô†‚Ìæ“¾
			[&]() { return targetNum; },
			// ƒAƒCƒhƒ‹ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE, true); }//,
			//// “¥‚İ‚Â‚¯‚Ö‚Ìó‘Ô‘JˆÚŠÖ”‚Ìƒ|ƒCƒ“ƒ^
			//[&]() { ChangeState((int)STATE::ATTACK_A); },
			//// “Š±‚Ö‚Ìó‘Ô‘JˆÚŠÖ”‚Ìƒ|ƒCƒ“ƒ^
			//[&]() { ChangeState((int)STATE::ATTACK_B); },
			//// ƒXƒ^ƒ“ƒv‚Ö‚Ìó‘Ô‘JˆÚŠÖ”‚Ìƒ|ƒCƒ“ƒ^
			//[&]() { ChangeState((int)STATE::ATTACK_C); },
			//// ‚Ğ‚Æ‚Â’Ç]‚Ö‚Ìó‘Ô‘JˆÚŠÖ”‚Ìƒ|ƒCƒ“ƒ^
			//[&]() { ChangeState((int)STATE::ATTACK_D); },
			//// ‚½‚­‚³‚ñ’Ç]‚Ö‚Ìó‘Ô‘JˆÚŠÖ”‚Ìƒ|ƒCƒ“ƒ^
			//[&]() { ChangeState((int)STATE::ATTACK_E); },
			//// ‚½‚­‚³‚ñƒ‰ƒ“ƒ_ƒ€‚Ö‚Ìó‘Ô‘JˆÚŠÖ”‚Ìƒ|ƒCƒ“ƒ^
			//[&]() { ChangeState((int)STATE::ATTACK_F); }
		)
	);

	AddState(
		static_cast<int>(STATE::DEATH),
		new BananaBossDeathState(
			// ©•ª‚Ìó‘Ô‚É‘JˆÚ‚·‚éŠÖ”
			[&]() { state = static_cast<int>(STATE::DEATH); },
			// ©•ª‚Ìó‘Ô‚©‚Ç‚¤‚©‚ğ•Ô‚·ŠÖ”
			[&]() { return state == static_cast<int>(STATE::DEATH); },
			// ƒ{ƒX‚ÌƒTƒCƒY
			trans.scale, MODEL_SCALE,
			// €‚ñ‚¾ƒtƒ‰ƒO‚ğ—§‚Ä‚é
			[&]() {	SetIsDeath(true); }
		)
	);

#pragma endregion


#pragma region ó‘Ôİ’è

#pragma endregion


#pragma region UI¶¬
	// HPƒo[¶¬
	ui_ArrayIns.emplace_back(
		new CharacterHpUI(
			characterStats.hp,
			characterStats.hpMax.Value(),
			HP_FRAME_IMAGE_NAME,
			HP_IMAGE_NAME,
			HP_LOST_IMAGE_NAME,
			HP_IMAGE_SIZE,
			HP_GAUGE_OFFSET,
			HP_UI_POS,
			FILE_PATH_TYPE::BOSS_HP,
			"BananaBoss"
		)
	);

	ui_ArrayIns.emplace_back(new HitUI(trans.pos));
#pragma endregion

}

void BananaBoss::ReceptionUpdate(void)
{
	BossBase::ReceptionUpdate();

}

void BananaBoss::CharacterInit(void)
{
	// ˆÊ’u‚ğ‰ŠúˆÊ’u‚É‚·‚é
	trans.pos = INIT_POS;
	trans.angle = INIT_ROT;

	// d—Í–³Œø
	ActorBase::SetGravityFlg(false);

	// ‰Šúó‘Ô‚ğˆÚ“®ó‘Ô‚É‚·‚é
	ChangeState(static_cast<int>(STATE::IDLE));

	for (ActorBase*& c : subObjArray) { c->Init(); }
}
