#pragma once
#include "GrapeBossWeaponBase.h"

#include "Straight/GrapeBossStraight.h"


class GrapeBossWeaponFactory {

public:

	static BombType Create(int model, int type) {

		BombType res;

		res.type = (WeaponType)type;
		switch ((WeaponType)type)
		{
		case WeaponType::None:
		{
			res.weaponIns = new GrapeBossStraight(model);
			break;
		}
		case WeaponType::Straight:
		{
			res.weaponIns = new GrapeBossStraight(model);
			break;
		}
		case WeaponType::KickBomb:
		{
			res.weaponIns = new GrapeBossStraight(model);
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
		case WeaponType::Max:
		{
			res.weaponIns = new GrapeBossStraight(model);
			break;
		}
		default:
			break;
		}
		return res;
	}
};