#pragma once
#include "GrapeBossWeaponBase.h"

#include "Straight/GrapeBossStraight.h"
#include "KickBomb/GrapeBossKickBomb.h"


class GrapeBossWeaponFactory {

public:

	// インスタンスの生産
	static BombType Create(int model, int type) {

		// 返り値用
		BombType res;

		// タイプの取得
		res.type = (WeaponType)type;

		// タイプに応じて生産
		switch ((WeaponType)type)
		{
		case WeaponType::Straight:
		{
			res.weaponIns = new GrapeBossStraight(model);
			break;
		}
		case WeaponType::KickBomb:
		{
			res.weaponIns = new GrapeBossKickBomb(model);
			break;
		}
		case WeaponType::StampBomb:
		{
			res.weaponIns = new GrapeBossStraight(model);
			break;
		}
		case WeaponType::SingleBomb:
		{
			res.weaponIns = new GrapeBossStraight(model);
			break;
		}
		case WeaponType::StalkerBomb:
		{
			res.weaponIns = new GrapeBossStraight(model);
			break;
		}
		case WeaponType::RandomBomb:
		{
			res.weaponIns = new GrapeBossStraight(model);
			break;
		}
		default:
			break;
		}

		// 生産品を返す
		return res;
	}
};