#include "TomatoStampCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

TomatoStampCollOperator::TomatoStampCollOperator
(
	float ATTACK_RADIUS, int ATTACK_DURATION, const bool& isGround, const Vector3& playerPos, const CharacterStats stats, const ParameterLoad& collParam
)	: ATTACK_RADIUS(ATTACK_RADIUS), ATTACK_DURATION(ATTACK_DURATION),
	ground(isGround), playerPos(playerPos), stats(stats),
	SCALE(collParam.GetParameterToVector3("Stamp","Scale")),
	HEIGHT(collParam.GetParameter("Stamp","Height")),
	collBack(Vector3()), collFront(Vector3())
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
	ColliderCreate(new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK, ATTACK_RADIUS));
	SetJudge(false);

	// 攻撃範囲の当たり判定
	ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).back()->SetDynamicFlg(true);
	ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).back()->SetPushFlg(true);
	ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).back()->SetJudgeFlg(true);

	CreateAttackSkill(100, &stats, COLLIDER_TAG::BOSS_ATTACK);

	collBack.Load("Range/CircleRangeBack");
	collFront.Load("Range/CircleRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT + 1.0f);

	collBack.scale = SCALE;
	collFront.scale = Vector3(0);
}

void TomatoStampCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)
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
		trans.pos.y = HEIGHT;
		collBack.pos = Vector3(trans.pos.x, collBack.pos.y, trans.pos.z);
		collFront.pos = Vector3(trans.pos.x, collFront.pos.y, trans.pos.z);
		end = false;
		attackCnt = 0;
	}
	else {
		if (ground) {
			if (attackCnt <= ATTACK_DURATION) {
				if (attackCnt != 0) {
					ColliderSerch(COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(true);
				}
				attackCnt++;
			}
			else {
				ColliderSerch(COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(false);
				end = true;
			}
		}
	}
}

void TomatoStampCollOperator::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collFront.Draw();
		collBack.Draw();
		SetUseLighting(true);
	}
}
