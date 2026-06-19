#include "TomatoStampCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

TomatoStampCollOperator::TomatoStampCollOperator
(
	float ATTACK_RADIUS,
	const bool& isGround,
	const std::vector<const Vector3*> playerPos,
	const MSG_SENDER_ID operatorSenderId,
	const CharacterStats& stats,
	const ParameterLoad& collParam
)	: 
	ATTACK_RADIUS(ATTACK_RADIUS),
	ground(isGround),
	playerPos(playerPos),
	operatorSenderId(operatorSenderId),
	stats(stats),
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
	// 1Pを初期値にしてる
	trans.pos = *playerPos.at(0);

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(false);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	// 当たり判定情報を生成する
	for (int i = 0; i < playerPos.size(); i++) {
		ColliderCreate(new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK_AREA, ATTACK_RADIUS));
		ColliderCreate(new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK, ATTACK_RADIUS));
	}
	SetJudge(false);

	// 攻撃範囲の当たり判定
	for (int i = 0; i < playerPos.size(); i++) {
		ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).at(i)->SetTransformPos(*playerPos.at(i));
		ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).at(i)->SetDynamicFlg(true);
		ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).at(i)->SetPushFlg(true);
		ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).at(i)->SetJudgeFlg(true);
	}

	CreateAttackSkill(operatorSenderId, 100, &stats, COLLIDER_TAG::BOSS_ATTACK);

	collBack.Load("Range/CircleRangeBack");
	collFront.Load("Range/CircleRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT + 1.0f);

	collBack.scale = SCALE;
	collFront.scale = Vector3(0);
}

void TomatoStampCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)
{
}

void TomatoStampCollOperator::SubUpdate(void)
{
	if (!isDrawArea) {
		for (int i = 0; i < playerPos.size(); i++) {
			ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).at(i)->SetTransformPos(*playerPos.at(i));
			ColliderSerch(COLLIDER_TAG::BOSS_ATTACK).at(i)->SetTransformPos(*playerPos.at(i));
		}
		trans.pos.y = HEIGHT;
		end = false;
		attackCnt = 0;
	}
	else {
		for (int i = 0; i < playerPos.size(); i++) {
			auto coll = ColliderSerch(COLLIDER_TAG::BOSS_ATTACK_AREA).at(i);
			if (coll->GetJudge()) {

				collBack.pos = Vector3(coll->GetPos().x, collBack.pos.y, coll->GetPos().z);
				collFront.pos = Vector3(coll->GetPos().x, collFront.pos.y, coll->GetPos().z);
			}
		}
	}
}

void TomatoStampCollOperator::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		SetUseLighting(true);
	}
}
