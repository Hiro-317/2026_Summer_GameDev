#include "GrapeBossWeaponFactory.h"

#include "Straight/GrapeBossStraight.h"
#include "KickBomb/GrapeBossKickBomb.h"
#include "StampBomb/GrapeBossStampBomb.h"
#include "SingleBomb/GrapeBossSingleBomb.h"
#include "StalkerBomb/GrapeBossStalkerBomb.h"
#include "RandomBomb/GrapeBossRandomBomb.h"

BombPoolEntry GrapeBossWeaponFactory::Create(int model, int type, int front, int back)
{
	// 返り値用
	BombPoolEntry res;

	// タイプの取得
	res.type = (WeaponType)type;

	// タイプに応じて生産
	switch ((WeaponType)type)
	{
	case WeaponType::Straight:
	{
		res.weaponActor = new GrapeBossStraight(model, front, back);
		break;
	}
	case WeaponType::KickBomb:
	{
		res.weaponActor = new GrapeBossKickBomb(model, front, back);
		break;
	}
	case WeaponType::StampBomb:
	{
		res.weaponActor = new GrapeBossStampBomb(model, front, back);
		break;
	}
	case WeaponType::SingleBomb:
	{
		res.weaponActor = new GrapeBossSingleBomb(model, front, back);
		break;
	}
	case WeaponType::StalkerBomb:
	{
		res.weaponActor = new GrapeBossStalkerBomb(model, front, back);
		break;
	}
	case WeaponType::RandomBomb:
	{
		res.weaponActor = new GrapeBossRandomBomb(model, front, back);
		break;
	}
	default:
		break;
	}

	// 生産品を返す
	return res;
}