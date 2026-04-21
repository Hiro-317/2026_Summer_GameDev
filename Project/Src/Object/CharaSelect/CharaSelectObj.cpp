#include "CharaSelectObj.h"

#include "../../Manager/Input/KeyManager.h"

CharaSelectObj::CharaSelectObj(void) : ActorBase("Data/Parameter/Charactor/Player/Select/SelectPlayerParameter.csv")
{
}

void CharaSelectObj::Load(void)
{

	// À•W
	trans.pos = INIT_POS;

	// Šp“x‚ÌƒYƒŒ(ƒ[ƒJƒ‹‰ñ“])
	trans.localAngle = MODEL_LOCAL_ROT;

	ActorBase::Init();

	// ƒAƒjƒ[ƒVƒ‡ƒ“````````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚ğ¶¬‚·‚é
	CreateAnimationController();

	// ŠeíƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	for (int i = 0; i < (int)ANIME_TYPE::MAX; i++) {
		AddAnimation(i, ANIME_SPEED_TABLE[i], ANIME_PATH_TABLE[i].c_str());
	}

	// ```````````````````````````````````
}

void CharaSelectObj::AddAnimation(int index, float speed, const char* filePath)
{
	anime->Add(index, speed, filePath);
}

void CharaSelectObj::SubUpdate(void)
{
	anime->Play(static_cast<int>(ANIME_TYPE::DANCE));
	anime->Update();

	nowChara = 0;

	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {

		nowChara--;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {

		nowChara++;
	}
}

void CharaSelectObj::SubRelease(void)
{
	anime->Release();
}
