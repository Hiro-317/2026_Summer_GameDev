#include "BananaBossBanadrill.h"

#include "../../../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../../UI/HitUI/HitUI.h"


BananaBossBanadrill::BananaBossBanadrill(
	const MSG_SENDER_ID operatorSenderId,
	const CharacterStats& stats
) :
	CharacterBase(1000, 0, 300, 0),
	operatorSenderId(operatorSenderId),
	stats(stats)
{
	banadrill.Load("Character/Banana/Banadrill");
	banadrill.scale = SIZE;
}

void BananaBossBanadrill::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK, ATTACK_RADIUS, ATTACK_RADIUS)
	);
	// ステージ当たり判定を生成する（球体コライダー）
	ColliderCreate(
		new XZCircleCollider(COLLIDER_TAG::ENEMY, RADIUS, RADIUS)
	);

	CreateAttackSkill(operatorSenderId, 85, &stats, COLLIDER_TAG::BOSS_ATTACK);
	ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetPushFlg(false);
	SetJudge(false);

	ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetTransformPtr(&banadrill);
	ColliderSerch(GetCollider(), COLLIDER_TAG::ENEMY).back()->SetTransformPtr(&banadrill);

	collBack.Load("Range/CircleRangeBack");
	collFront.Load("Range/CircleRangeFront");

	collBack.scale = SCALE;
	collFront.scale = Vector3(0.0f);

	isDrawArea = false;
}

void BananaBossBanadrill::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (ownTag == COLLIDER_TAG::ENEMY) {
		if (other.GetTag() == COLLIDER_TAG::PLAYER_ATTACK) {
			bool isClitical = false;
			short damage = CalculateDamage(other.GetSkillStats().Power(&isClitical), characterStats.defensePower.Value());

			// ホストならそのままUI描画
			if (other.GetSkillStats().operatorSenderId == Net::GetIns().GetSenderId()) {
				SubUiSerch<HitUI>()->DamageSetting(collisionPoint, damage, isClitical);
			}

			// ダメージ減らす
			characterStats.hp -= damage;

			// ダメージとクリティカル情報を送信する
			Net::GetIns().Send(MsgDataBossHit(collisionPoint, damage, isClitical), other.GetSkillStats().operatorSenderId);

			// HPがゼロ以下になったら吹っ飛ぶ状態に遷移
		}
	}
}

void BananaBossBanadrill::SubUpdate(void)
{
	// 起動
	if (isDrawArea) {
		// 空中なら加速し予測線の拡大
		if (!isGround) {
			collFront.scale = SCALE * ((ATTACK_HEIGHT - banadrill.pos.y) / (ATTACK_HEIGHT - HEIGHT));
			banadrill.pos.y -= 10.0f;
		}
		else {
			// 地面でぐるぐるする時間
			if (cnt == 0) {
				collFront.scale = SCALE;
			}
			cnt++;
			if (cnt > ATTACK_DURATION) {
				isDrawArea = false;
				ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(false);;
				cnt = 0;
			}
		}
	}
}

void BananaBossBanadrill::SubDraw(void)
{
	if (live) {
		banadrill.Draw();
	}
}

void BananaBossBanadrill::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		SetUseLighting(true);
	}
}
