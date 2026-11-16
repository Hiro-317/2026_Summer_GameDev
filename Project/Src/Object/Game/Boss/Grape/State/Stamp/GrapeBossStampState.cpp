#include "GrapeBossStampState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

GrapeBossStampState::GrapeBossStampState(
	Transform& bossTrans,
	const bool& isGround,
	BombPoolEntry* bombPool,
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
	bombPool(bombPool), playerPos(playerPos),
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
	activeAttackFrames(0), isLandingAttackPending(false),
	isJumpPending(false), horizontalMoveFrames(0),
	previousHeight(0.0f), targetPlayerIndex(0)
{
}

void GrapeBossStampState::Enter(void)
{
	targetPlayerIndex = GetTarget();
	if (targetPlayerIndex < 0 || targetPlayerIndex >= (int)MSG_SENDER_ID::Max || playerPos[targetPlayerIndex] == nullptr) { return; }
	collOperator->SetPos(*playerPos[targetPlayerIndex]);
	attackPos = Vector3();
	horizontalMoveStep = Vector3();
	collOperator->DrawAreaOn();
	horizontalMoveFrames = 0;
	previousHeight = bossTrans.pos.y - 0.5f;
	isJumpPending = true;
	SetCoolTime();
	PlayJumpStartAnim();
	isLandingAttackPending = true;
	activeAttackFrames = 0;

	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC));
	Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, *playerPos[targetPlayerIndex]));
}

void GrapeBossStampState::Update(void)
{
	if (targetPlayerIndex < 0 || targetPlayerIndex >= (int)MSG_SENDER_ID::Max || playerPos[targetPlayerIndex] == nullptr) { return; }

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
			return;
		}
	}

	if (!isGround || isJumpPending) {
		if (isJumpPending) {
			isJumpPending = false;
		}
		if (previousHeight < bossTrans.pos.y) {
			previousHeight = bossTrans.pos.y;
			bossTrans.pos.y += JUMP_POW;
			offCollider();
		}
		else {
			previousHeight = bossTrans.pos.y;
			onCollider();
		}
	}
	else {
		// 一回だけ通る
		if (isLandingAttackPending) {
			// 地上についたのでスタンプ
			PlayStampAnim();
			Snd::GetIns().Play("StampLand");
			// 生成数まで回す
			for (int i = 0; i < WeaponPoolCounts[(int)WeaponType::StampBomb]; i++) {

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
				bombPool[i].weaponActor->SetStartPos(tmpPos);
				bombPool[i].weaponActor->SetColliderFlg(true);
				bombPool[i].weaponActor->SetViewPosCircle();

				// 起動を通知
				Net::GetIns().Send(MsgDataBossBombInform(MsgDataBossBombInform::INFORM_TYPE::StampBomb, i, tmpPos));

				// 起動
				bombPool[i].isActive = true;
			}
			// 位置に攻撃を発生
			collOperator->CollOn();
		}
		// 二度目は通さないようにフラグを折る
		isLandingAttackPending = false;

		// 攻撃持続時間の加算
		activeAttackFrames++;

		// 攻撃持続時間の判定
		if (activeAttackFrames > ATTACK_DURATION) {
			// フレーム位置に攻撃を消去
			collOperator->Off();
			Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC, false));
			collOperator->SetScale(0.0f);
			Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, attackPos, 0.0f));
		}
	}

	if (horizontalMoveFrames < TIME_RATE) {

		attackPos = collOperator->GetAttackPos();
		horizontalMoveStep = Vector3::XZonly(attackPos.x - bossTrans.pos.x, attackPos.z - bossTrans.pos.z) / (TIME_RATE - (float)horizontalMoveFrames);

		horizontalMoveFrames++;
		bossTrans.pos += horizontalMoveStep;

		// 予測線の更新
		float scale = 1.0f - (TIME_RATE - (float)horizontalMoveFrames) / TIME_RATE;
		collOperator->SetScale(scale);
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, attackPos, scale));
	}
}

void GrapeBossStampState::Exit(void)
{
	// 着地前に中断されても本体の判定を復帰し、攻撃判定を解除する。
	onCollider();
	collOperator->Off();
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC, false));
}

void GrapeBossStampState::AlwaysUpdate(void)
{
}
