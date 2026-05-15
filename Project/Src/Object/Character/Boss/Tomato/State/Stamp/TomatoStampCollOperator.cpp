#include "TomatoStampCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

TomatoStampCollOperator::TomatoStampCollOperator
(
	float ATTACK_RADIUS, int ATTACK_DURATION, const bool& isGround, const Vector3& playerPos, const CharacterStats stats
)	: ATTACK_RADIUS(ATTACK_RADIUS), ATTACK_DURATION(ATTACK_DURATION), ground(isGround), playerPos(playerPos), stats(stats)
{
	isDrawArea = false;
	end = false;
	attackCnt = 0;
	trans.pos = Vector3(-1, -1, -1);
}

void TomatoStampCollOperator::Load(void)
{
	trans.pos = playerPos;

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(false);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	// 当たり判定情報を生成する
	ColliderCreate(new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK_AREA, ATTACK_RADIUS));
	ColliderCreate(new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK_1, ATTACK_RADIUS));
	SetJudge(false);

	// 攻撃範囲の当たり判定
	ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).back()->SetDynamicFlg(true);
	ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).back()->SetPushFlg(true);
	ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).back()->SetJudgeFlg(true);

	CreateAttackSkill(100, &stats, COLLIDER_TAG::BOSS_ATTACK_1);
}

void TomatoStampCollOperator::Draw(void)
{
	if (!GetIsDraw()) {
		return;
	}
	if (isDrawArea) {
		DrawCone3D((trans.pos + Vector3::Yonly(30.0f)).ToVECTOR(), (trans.pos + Vector3::Yonly(25.0f)).ToVECTOR(), ATTACK_RADIUS, 64, 0xdd9900, 0xffffff, true);
	}
}

void TomatoStampCollOperator::OnCollision(const ColliderBase& other)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::PLAYER:
		break;
	default:break;
	}
}

void TomatoStampCollOperator::SubUpdate(void)
{
	if (!isDrawArea) {
		trans.pos = playerPos;
		trans.pos.y = 0.0f;
		end = false;
		attackCnt = 0;
	}
	else {
		if (ground) {
			ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_1).back()->SetJudgeFlg(true);

			if (attackCnt < ATTACK_DURATION) {
				attackCnt++;
			}
			else {
				end = true;
			}
		}
	}
}



























































































































































































































// ちんこ