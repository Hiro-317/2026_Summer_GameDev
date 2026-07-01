#include "GrapeBossWeaponManager.h"

#include "Straight/GrapeBossStraight.h"


GrapeBossWeaponManager::GrapeBossWeaponManager()
{
	bombModel = MV1LoadModel("Data/Model/Charactor/Grape/Bomb.mv1");
}

GrapeBossWeaponManager::~GrapeBossWeaponManager()
{
}

void GrapeBossWeaponManager::Init() 
{
	int weaponNumber = 0;
	for (int i = 0; i < (int)WeaponType::Max; i++) {
		for (int j = 0; j < WeponDuplicateNum[i]; j++) {

			weapons[weaponNumber].type = (WeaponType)i;
			switch ((WeaponType)i)
			{
			case GrapeBossWeaponManager::WeaponType::Straight:
			{
				weapons[weaponNumber].weaponIns = new GrapeBossStraight(bombModel);
				break;
			}
			case GrapeBossWeaponManager::WeaponType::KickBomb:
			{
				weapons[weaponNumber].weaponIns = new GrapeBossStraight(bombModel);
				break;
			}
			case GrapeBossWeaponManager::WeaponType::StampBomb:
			{
				weapons[weaponNumber].weaponIns = new GrapeBossStraight(bombModel);
				break;
			}
			case GrapeBossWeaponManager::WeaponType::SingleBomb:
			{
				weapons[weaponNumber].weaponIns = new GrapeBossStraight(bombModel);
				break;
			}
			case GrapeBossWeaponManager::WeaponType::StalkerBomb:
			{
				weapons[weaponNumber].weaponIns = new GrapeBossStraight(bombModel);
				break;
			}
			case GrapeBossWeaponManager::WeaponType::RandomBomb:
			{
				weapons[weaponNumber].weaponIns = new GrapeBossStraight(bombModel);
				break;
			}
			default:
				break;
			}
			weaponNumber++;
		}
	}
	MV1DeleteModel(bombModel);
}

void GrapeBossWeaponManager::Update()
{
	for (auto& i : weapons) i.weaponIns->Update();
}

void GrapeBossWeaponManager::Draw()
{
	for (auto& i : weapons) i.weaponIns->Draw();
}

void GrapeBossWeaponManager::Release() 
{
	int weaponNumber = 0;
	for (int i = 0; i < (int)WeaponType::Max; i++) {
		for (int j = 0; j < WeponDuplicateNum[i]; j++) {

			weapons[weaponNumber].weaponIns->Release();
			delete weapons[weaponNumber].weaponIns;
			weaponNumber++;
		}
	}
	delete weapons;
}