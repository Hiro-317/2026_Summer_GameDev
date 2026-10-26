#include "GrapeBossStampState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

GrapeBossStampState::GrapeBossStampState(
	Transform& bossTrans,
	const bool& isGround,
	BombType* bombType,
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	GrapeStampCollOperator* collOperator,
	const int JUMP_START_ANIM_TYPE,
	const int STAMP_ANIM_TYPE,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayJumpStartAnim,
	const std::function<void(void)> PlayJumpAnim,
	const std::function<void(void)> PlayStampAnim,
	const std::function<int(void)> GetAnimType,
	const std::function<float(void)> GetAnimPlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> offCollider,
	const std::function<void(void)> onCollider,
	const std::function<void(void)> SetCoolTime
) :
	bossTrans(bossTrans),
	isGround(isGround),
	bombType(bombType), playerPos(playerPos),
	collOperator(collOperator),
	JUMP_START_ANIM_TYPE(JUMP_START_ANIM_TYPE),
	STAMP_ANIM_TYPE(STAMP_ANIM_TYPE),
	GetTarget(GetTarget),
	PlayJumpStartAnim(PlayJumpStartAnim),
	PlayJumpAnim(PlayJumpAnim),
	PlayStampAnim(PlayStampAnim),
	GetAnimType(GetAnimType),
	GetAnimPlayRatio(GetAnimPlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	offCollider(offCollider),
	onCollider(onCollider),
	SetCoolTime(SetCoolTime),
	attackCnt(0), cnt(0), first(false),
	isAttack(false), nowAttackTime(0),
	prevPos(0.0f), target(0)
{
}

void GrapeBossStampState::Enter(void)
{
	target = GetTarget();
	collOperator->SetPos(*playerPos[target]);
	attackPos = Vector3();
	attackDistRate = Vector3();
	collOperator->DrawAreaOn();
	nowAttackTime = 0;
	prevPos = bossTrans.pos.y - 0.5f;
	attackCnt = 0;
	isAttack = true;
	SetCoolTime();
	PlayJumpStartAnim();
	first = true;
	cnt = 0;

	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC));
	Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, *playerPos[target]));
}

void GrapeBossStampState::Update(void)
{
	// ジャンプはじめの時
	if (GetAnimType() == JUMP_START_ANIM_TYPE) {
		// 踏み切るまでスキップ
		if (GetAnimPlayRatio() < 0.65f) {
			return;
		}
		// 踏み切り終わりは持続アニメーション
		if (IsAnimeEnd()) {
			PlayJumpAnim();
			Snd::GetIns().Play("StampJump");
		}
	}
	// スタンプ
	if (GetAnimType() == STAMP_ANIM_TYPE) {
		if (IsAnimeEnd()) {

			DefaultChangeState();
		}
	}

	if (!isGround || isAttack) {
		if (isAttack) {
			isAttack = false;
		}
		if (prevPos < bossTrans.pos.y) {
			prevPos = bossTrans.pos.y;
			bossTrans.pos.y += JUMP_POW;
			offCollider();
		}
		else {
			prevPos = bossTrans.pos.y;
			onCollider();
		}
	}
	else {
		// 一回だけ通る
		if (first) {
			// 地上についたのでスタンプ
			PlayStampAnim();
			Snd::GetIns().Play("StampLand");
			// 生成数まで回す
			for (int i = 0; i < WeponDuplicateNum[(int)WeaponType::StampBomb]; i++) {

				// 前後左右の制定
				float FB = (float)GetRand(RADIUS);
				float LR = (float)GetRand(RADIUS);

				// ランダム位置の生成
				float posX = 0.0f;
				float posZ = 0.0f;

				// 半分の確率で前・後ろ
				if (FB > RADIUS / 2) {
					posX = -(float)GetRand(RADIUS) - (float)RADIUS;
				}
				else {
					posX = (float)GetRand(RADIUS) + (float)RADIUS;
				}

				// 半分の確率で左・右
				if (LR > RADIUS / 2) {
					posZ = -(float)GetRand(RADIUS) - (float)RADIUS;
				}
				else {
					posZ = (float)GetRand(RADIUS) + (float)RADIUS;
				}

				// スタート位置の設定
				Vector3 tmpPos = Vector3(bossTrans.pos.x + posX, HEIGHT, bossTrans.pos.z + posZ);
				bombType[i].weaponIns->SetStartPos(tmpPos);
				bombType[i].weaponIns->SetViewPosCircle();

				// 起動を通知
				Net::GetIns().Send(MsgDataBossBombInform(MsgDataBossBombInform::INFORM_TYPE::StampBomb, i, tmpPos));

				// 起動
				bombType[i].live = true;
			}
			// 位置に攻撃を発生
			collOperator->CollOn();
		}
		// 二度目は通さないようにフラグを折る
		first = false;
		
		// 攻撃持続時間の加算
		cnt++;

		// 攻撃持続時間の判定
		if (cnt > ATTACK_DURATION) {
			// フレーム位置に攻撃を消去
			collOperator->Off();
			Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC, false));
			collOperator->SetScale(0.0f);
			Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, attackPos, 0.0f));
		}
	}

	if (nowAttackTime < TIME_RATE) {

		attackPos = collOperator->GetAttackPos();
		attackDistRate = Vector3::XZonly(attackPos.x - bossTrans.pos.x, attackPos.z - bossTrans.pos.z) / (TIME_RATE - (float)nowAttackTime);

		nowAttackTime++;
		bossTrans.pos += attackDistRate;

		// 予測線の更新
		float scale = 1.0f - (TIME_RATE - (float)nowAttackTime) / TIME_RATE;
		collOperator->SetScale(scale);
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, attackPos, scale));
	}
}

void GrapeBossStampState::Exit(void)
{
}

void GrapeBossStampState::AlwaysUpdate(void)
{
}
