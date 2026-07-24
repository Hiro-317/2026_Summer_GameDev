#include "GrapePlayerShotCollOperator.h"

#include "../../../../../../../Manager/Effect/EffectManager.h"
#include "../../../../../../Common/Collider/SphereCollider.h"

GrapePlayerShotCollOperator::GrapePlayerShotCollOperator(
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
	lifeCounter(0)
{
}

void GrapePlayerShotCollOperator::Load(void)
{
#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	// コライダー生成
	ColliderCreate(new SphereCollider(COLL_TAG, 50.0f));

	// 初期化処理
	SetJudge(false);
	isHit = false;

	// 最初は描画しない
	SetIsDraw(false);

	// スキルのダメージ量の設定
	CreateAttackSkill(operatorSenderId, ATTACK_RATE_PERCENT, &playerStats, COLL_TAG);

	// 弾のモデルをロード、スケールの設定
	trans.Load("Character/Grape/Bomb");
	trans.scale = 0.1f;

	// 発射地点を初期化、プレイヤーの足元から少し上から発射するようにする
	trans.pos.y = trans.pos.y + 30.0f;

	// カウンタを設定
	lifeCounter = LIFE_TIME;
}

void GrapePlayerShotCollOperator::Update(void)
{
	// 撃った瞬間から弾の生存時間を減らす
	if (lifeCounter > 0) {
		lifeCounter--;
	}

	// 生存時間がなくなるか、敵に当たったら弾を消す処理を行う
	if (lifeCounter <= 0 || isHit) {
		lifeCounter = 0;
		CollOff();
		SetIsDraw(false);
		return;
	}

	// 弾を飛ばす処理
	if (GetIsDraw()) {
		trans.pos += targetVec * 40.0f;
		CollOn();
	}
}

void GrapePlayerShotCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
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