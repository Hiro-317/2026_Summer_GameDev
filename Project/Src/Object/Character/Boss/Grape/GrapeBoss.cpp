#include "GrapeBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Font/FontManager.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"

#include "../../../../Scene/Game/GameScene.h"

#include "../../../../Manager/Effect/EffectManager.h"

#include "../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../UI/HitUI/HitUI.h"


GrapeBoss::GrapeBoss(const std::vector<const Vector3*> playerPos) :
	BossBase(
		"GrapeBossParameter",
		"GrapeBossHP",
		"GrapeBossAttackPower",
		"GrapeBossDefensePower",
		"GrapeBossMoveSpeed",
		"Data/Parameter/Character/Boss/Tomato/",
		"Grape/Grape",

		playerPos)
{

	coolTime = 120;

	this->operatorSenderId = Net::HOST_SENDER_ID;
	isOwnOperator = true;

	rockHit = false;
}

void GrapeBoss::PlayerLoad(void)
{

#pragma region 当たり判定情報設定

	// 当たり判定を生成する（カプセルコライダー）
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::BOSS,
			CAPSULE_COLLIDER_START_POS, CAPSULE_COLLIDER_END_POS,
			CAPSULE_COLLIDER_RADIUS,
			CAPSULE_COLLIDER_ENOUGH_DISTANCE
		)
	);

#pragma endregion


#pragma region プレイヤーが抱える下位クラスを生成する


#pragma endregion


#pragma region 状態設定


#pragma endregion


#pragma region UI生成
	// HPバー生成
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
			"GrapeBoss"
		)
	);

	ui_ArrayIns.emplace_back(new HitUI());
#pragma endregion

}

void GrapeBoss::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)
{

}