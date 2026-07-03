#include "GrapeBossWeaponManager.h"

#include "GrapeBossWeaponFactory.h"


GrapeBossWeaponManager::GrapeBossWeaponManager(const MSG_SENDER_ID& operatorSenderId, const CharacterStats& stats)
	: operatorSenderId(operatorSenderId), stats(stats)
{
	// 複製ハンドル
	bombModel = MV1LoadModel("Data/Model/Charactor/Grape/Bomb.mv1");
}

GrapeBossWeaponManager::~GrapeBossWeaponManager()
{
}

void GrapeBossWeaponManager::Load(void)
{
	// 初期化用の数
	int weaponNumber = 0;

	// 武器の種類数とその中身の数まで回す
	for (int i = 0; i < (int)WeaponType::Max; i++) {
		for (int j = 0; j < WeponDuplicateNum[i]; j++) {

			// 作ってもらった武器を格納
			weapons[weaponNumber] = GrapeBossWeaponFactory::Create(bombModel, i);

			// 数の更新
			weaponNumber++;
		}
	}
	// 複製用ハンドルの消去
	MV1DeleteModel(bombModel);

	// 武器の初期化
	for (auto& i : weapons) i.weaponIns->Load(operatorSenderId, stats);
}

void GrapeBossWeaponManager::SubUpdate()
{
	for (auto& i : weapons) {
		if (i.live) {
			i.weaponIns->Update();
		}
	}
}

void GrapeBossWeaponManager::SubDraw()
{
	for (auto& i : weapons) {
		if (i.live) {
			i.weaponIns->Draw();
		}
	}
}

void GrapeBossWeaponManager::SubRelease() 
{
	for (auto& i : weapons) i.weaponIns->Release();
}