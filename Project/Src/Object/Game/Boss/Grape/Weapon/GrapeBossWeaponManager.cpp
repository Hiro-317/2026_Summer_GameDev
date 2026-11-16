#include "GrapeBossWeaponManager.h"

#include "../../../../../Manager/Sound/SoundManager.h"

#include "GrapeBossWeaponFactory.h"
#include "GrapeBossWeaponBase.h"


GrapeBossWeaponManager::GrapeBossWeaponManager(const MSG_SENDER_ID& operatorSenderId, const CharacterStats& stats)
	: operatorSenderId(operatorSenderId), stats(stats)
{
	// 複製ハンドル
	bombModel = MV1LoadModel("Data/Model/Character/Grape/Bomb.mv1");
	frontModel = MV1LoadModel("Data/Model/Range/CircleRangeFront.mv1");
	backModel = MV1LoadModel("Data/Model/Range/CircleRangeBack.mv1");

	// 初期化用の数
	int weaponNumber = 0;

	// 武器の種類数とその中身の数まで回す
	for (int i = 0; i < (int)WeaponType::Max; i++) {
		for (int j = 0; j < WeaponPoolCounts[i]; j++) {

			// 作ってもらった武器を格納
			weapons[weaponNumber] = GrapeBossWeaponFactory::Create(bombModel, i, frontModel, backModel);

			AddChildActor(weapons[weaponNumber].weaponActor);

			weapons[weaponNumber].weaponActor->SetColliderFlg(false);
			weapons[weaponNumber].isActive = false;

			// 数の更新
			weaponNumber++;
		}
	}
}

void GrapeBossWeaponManager::Load(void)
{
	// 武器の初期化
	for (auto& i : weapons) i.weaponActor->Load(stats);
}

void GrapeBossWeaponManager::Update()
{
	for (auto& i : weapons) {
		// 非生存の弾は移動・攻撃を止めるが、子の爆発エフェクトは更新を続ける。
		if (!i.isActive) {
			i.weaponActor->UpdateInactiveEffects();
			continue;
		}
		if (i.isActive) {
			i.weaponActor->Update();

			// 終了しているなら殺す
			if (i.weaponActor->IsEnd()) {

				i.isActive = false;
				i.weaponActor->ResetEnd();
				i.weaponActor->SetColliderFlg(false);
				Snd::GetIns().Play("ExGrape");
			}
			// 例外処理
			if (i.weaponActor->GetTrans().pos.y < EXIT_HEIGHT) {

				i.isActive = false;
				i.weaponActor->SetColliderFlg(false);
				i.weaponActor->ResetEnd();
			}
		}
	}
}

void GrapeBossWeaponManager::Draw()
{
	for (auto& i : weapons) {
		if (i.isActive) {
			i.weaponActor->Draw();
		}
	}
}

void GrapeBossWeaponManager::Release()
{
	for (auto& i : weapons) {
		i.weaponActor = nullptr;
	}
	MV1DeleteModel(bombModel);
	MV1DeleteModel(frontModel);
	MV1DeleteModel(backModel);
}