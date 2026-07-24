#include "GrapePlayerThrowCollOperator.h"

#include "../../../../../../../Manager/Effect/EffectManager.h"
#include "../../../../../../Common/Collider/SphereCollider.h"

GrapePlayerThrowCollOperator::GrapePlayerThrowCollOperator(
	COLLIDER_TAG COLL_TAG,
	const short ATTACK_RATE_PERCENT,
	const Vector3& playerPos, const Vector3& playerAngle,
	MSG_SENDER_ID operatorSenderId,
	const CharacterStats& playerStats
) :
	ActorBase(),
	COLL_TAG(COLL_TAG),
	ATTACK_RATE_PERCENT(ATTACK_RATE_PERCENT),
	playerPos(playerPos), playerAngle(playerAngle),
	operatorSenderId(operatorSenderId),
	playerStats(playerStats),
	isHit(false),
	targetVec(Vector3()),
	gravity(0),
	bouncePower(0.0f)
{
}

void GrapePlayerThrowCollOperator::Load(void)
{


#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(true);
#pragma endregion

	// コライダー生成
	ColliderCreate(new SphereCollider(COLL_TAG, 100.0f));

	// 初期化処理
	SetJudge(false);
	isHit = false;

	// 最初は描画しない
	SetIsDraw(false);

	// スキルのダメージ量の設定
	CreateAttackSkill(operatorSenderId, ATTACK_RATE_PERCENT, &playerStats, COLL_TAG);

	trans.Load("Character/Grape/Bomb");
	trans.scale = 0.5f;
}

void GrapePlayerThrowCollOperator::SubUpdate()
{
	// 重力を加える
	gravity -= 0.5f;

	// ヒットしたら消える
	if (isHit) {
		CollOff();
		SetIsDraw(false);
		return;
	}

	if (GetIsDraw()) {
		CollOn();

		if (isGround) {
			bouncePower /= 1.5f;
			gravity = bouncePower;

			if (bouncePower <= 0.1f) {
				SetPushFlg(false);
				ColliderSerch<SphereCollider>(COLL_TAG).back()->SetRadius(300.0f);
				bouncePower = 0.0f;
				EffectManager::GetIns()->CreateEffect(EFFECT_NAME::BOMB_BIG, trans.pos);
				SetIsDraw(false);
				isHit = true;
				return;
			}
		}
		else {
			// 前方向へ移動
			trans.pos += targetVec * 10.0f;
		}

		// 縦方向へ移動
		trans.pos.y += gravity;
	}
}

void GrapePlayerThrowCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (ownTag == COLL_TAG) {
		// 攻撃の当たり判定
		switch (other.GetTag())
		{
		case COLLIDER_TAG::BOSS:
		case COLLIDER_TAG::ENEMY:
		case COLLIDER_TAG::BOSS_DISTANCE:
			isHit = true;
			EffectManager::GetIns()->CreateEffect(EFFECT_NAME::BOMB_SMALL, trans.pos);
			SetIsDraw(false);
			break;

		default:break;
		}
	}
}

void GrapePlayerThrowCollOperator::SetInit(void)
{
	ColliderSerch<SphereCollider>(COLL_TAG).back()->SetRadius(100.0f);
	SetPushFlg(true);
	trans.pos = playerPos;
	trans.pos.y = trans.pos.y + 100.0f;
	SetIsDraw(true);
	bouncePower = 10.0f;
	gravity = bouncePower;
	ResetIsHit();
	CollOff();
}