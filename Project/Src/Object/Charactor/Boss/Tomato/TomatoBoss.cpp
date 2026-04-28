#include "TomatoBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Font/FontManager.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/SphereCollider.h"

#include "State/Move/TomatoMove.h"

TomatoBoss::TomatoBoss(const Vector3& playerPos) :
	CharactorBase(1,1,1,1,"Data/Parameter/Charactor/Boss/Tomato/TomatoBossParameter.csv"),
	subObjArray(),
	playerPos(playerPos)
{
}


void TomatoBoss::Load(void)
{
#pragma region ƒ‚ƒfƒ‹

	// ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
	trans.Load("Charactor/Tomato/Tomato");

	// ƒ‚ƒfƒ‹‚ÌƒXƒP[ƒ‹‚ğİ’è
	trans.scale = MODEL_SCALE;

	// ƒ‚ƒfƒ‹‚Ì’†S“_‚ÌƒYƒŒ
	trans.centerDiff = MODEL_CENTER_DIFF;

	// Šp“x‚ÌƒYƒŒ(ƒ[ƒJƒ‹‰ñ“])
	trans.localAngle = MODEL_LOCAL_ROT;

	// ƒAƒjƒ[ƒVƒ‡ƒ“````````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚ğ¶¬‚·‚é
	CreateAnimationController();

	// ```````````````````````````````````

#pragma endregion


#pragma region Šî’êƒNƒ‰ƒX‚É‚ ‚é‹@”\‚Ì‹““®İ’è

	// “®“IƒIƒuƒWƒFƒNƒg‚Æ‚µ‚Ä‚Ì‹““®‚ğ—LŒø‚É‚·‚é
	SetDynamicFlg(true);

	// d—Í‚ğ—LŒø‚É‚·‚é
	SetGravityFlg(true);

	// Õ“Ë‚Ì‰Ÿ‚µo‚µ‚ğ—LŒø‚É‚·‚é
	SetPushFlg(true);

	// ‰Ÿ‚µo‚·—Í‚Ì‘å‚«‚³‚ğİ’è‚·‚é
	SetPushWeight(COLLISION_PUSH_WEIGHT);

#pragma endregion


#pragma region “–‚½‚è”»’èî•ñİ’è

	//// “–‚½‚è”»’è‚ğ¶¬‚·‚éiü•ªƒRƒ‰ƒCƒ_[j
	//ColliderCreate(new LineCollider(TAG::PLAYER, LINE_COLLIDER_START_POS, LINE_COLLIDER_END_POS));

	// “–‚½‚è”»’è‚ğ¶¬‚·‚éiƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_[j
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::BOSS,
			CAPSULE_COLLIDER_START_POS_X, CAPSULE_COLLIDER_END_POS_X,
			CAPSULE_COLLIDER_RADIUS,
			CAPSULE_COLLIDER_ENOUGH_DISTANCE
		)
	);
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::BOSS,
			CAPSULE_COLLIDER_START_POS_Z, CAPSULE_COLLIDER_END_POS_Z,
			CAPSULE_COLLIDER_RADIUS,
			CAPSULE_COLLIDER_ENOUGH_DISTANCE
		)
	);

#pragma endregion


#pragma region ƒvƒŒƒCƒ„[‚ª•ø‚¦‚é‰ºˆÊƒNƒ‰ƒX‚ğ¶¬‚·‚é

	// ‚Ü‚Æ‚ß‚Ä“Ç‚İ‚İˆ—
	for (ActorBase*& c : subObjArray) { c->Load(); }
#pragma endregion



#pragma region ó‘Ôİ’è

	AddState(
		static_cast<int>(STATE::MOVE),
		new TomatoMove(
			// ©•ª‚Ìó‘Ô‚É‘JˆÚ‚·‚éŠÖ”
			[&]() { state = static_cast<int>(STATE::MOVE); },
			// ©•ª‚Ìó‘Ô‚©‚Ç‚¤‚©‚ğ•Ô‚·ŠÖ”
			[&]() { return state == static_cast<int>(STATE::MOVE); },
			// ˆÚ“®—Ê‚Æ‰ñ“]—Ê
			MOVE_SPEED, ROTATION_POW,
			// ©•ª‚ÌÀ•W‚ÆŠp“xAƒvƒŒƒCƒ„[‚ÌÀ•W‚Ì“Ç‚İæ‚è
			trans.pos, trans.angle, playerPos
			)
	);

#pragma endregion
}

void TomatoBoss::CharactorInit(void)
{
	// ˆÊ’u‚ğ‰ŠúˆÊ’u‚É‚·‚é
	trans.pos = INIT_POS;

	// ‰Šúó‘Ô‚ğˆÚ“®ó‘Ô‚É‚·‚é
	state = (int)STATE::MOVE;

	for (ActorBase*& c : subObjArray) { c->Init(); }

}

void TomatoBoss::CharactorUpdate(void)
{
	for (ActorBase*& c : subObjArray) { c->Update(); }
}

void TomatoBoss::CharactorDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->Draw(); }
}

void TomatoBoss::CharactorAlphaDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->AlphaDraw(); }
}

void TomatoBoss::UiDraw(void)
{
	if (App::GetIns().IsDrawDebug()) {

		// 1s‚¸‚Â•`‰æ‚·‚é‚½‚ß‚Ìƒ‰ƒ€ƒ_®iƒfƒoƒbƒO—pj
		int yPos = 300; const int FONT_SIZE = 20;
		auto debugDrwStr = [&](std::string str)->void {
			DrawStringToHandle(0, yPos, str.c_str(), 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_20));
			yPos += FONT_SIZE;
			};

		// ‰Á‘¬“x‚ğƒfƒoƒbƒO•\¦
		debugDrwStr("ƒ{ƒX`````````");
		debugDrwStr("À•W" + std::to_string(trans.pos.x) + ", " + std::to_string(trans.pos.y) + ", " + std::to_string(trans.pos.z));
		debugDrwStr("‰Á‘¬“x:" + std::to_string(accelSum.Length()));
		debugDrwStr("```````(|3[___]");
	}
}


void TomatoBoss::CharactorRelease(void)
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
