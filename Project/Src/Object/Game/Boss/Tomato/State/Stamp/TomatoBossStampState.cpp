#include "TomatoBossStampState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"


TomatoBossStampState::TomatoBossStampState(
	TomatoStampCollOperator* collOperator,
	Vector3& pos, Quaternion& rotation, const bool& isGround,
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> offCollider,
	const std::function<void(void)> onCollider,
	const std::function<void(void)> SetCoolTime
) :CharacterStateBase(),
	collOperator(collOperator),
	pos(pos), rotation(rotation), isGround(isGround),
	playerPos(playerPos),
	GetTarget(GetTarget),
	DefaultChangeState(DefaultChangeState),
	offCollider(offCollider),
	onCollider(onCollider),
	SetCoolTime(SetCoolTime)
{
	previousHeight = pos.y;
	horizontalMoveFrames = 0;
	activeAttackFrames = 0;
	isJumpPending = true;
}

void TomatoBossStampState::Enter(void)
{
	targetPlayerIndex = GetTarget();
	if (targetPlayerIndex < 0 || targetPlayerIndex >= (int)MSG_SENDER_ID::Max || playerPos[targetPlayerIndex] == nullptr) { return; }
	collOperator->SetPos(*playerPos[targetPlayerIndex]);
	attackPos = Vector3();
	horizontalMoveStep = Vector3();
	collOperator->DrawAreaOn();
	horizontalMoveFrames = 0;
	previousHeight = pos.y - 0.5f;
	activeAttackFrames = 0;
	isJumpPending = true;
	SetCoolTime();
	Vector3 tmp = *playerPos[targetPlayerIndex] - pos;
	rotation = Quaternion::FromRotationY(atan2f(tmp.x, tmp.z));

	SoundManager::GetIns().Play("TomatoPlayerStampJump");
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB));
}

void TomatoBossStampState::Update(void)
{
	if (targetPlayerIndex < 0 || targetPlayerIndex >= (int)MSG_SENDER_ID::Max || playerPos[targetPlayerIndex] == nullptr) { return; }

	// 地面にいなく初めなら
	if (!isGround || isJumpPending) {
		// 初めのフラグを落とす
		if (isJumpPending) {
			isJumpPending = false;
		}
		// 重力に負けるまで上げ
		// ボス自体のコライダを消す
		if (previousHeight < pos.y) {
			previousHeight = pos.y;
			pos.y += JUMP_POW;
			offCollider();
		}
		// 重力に任せる
		// ボス自体のコライダを復活させる
		else {
			previousHeight = pos.y;
			onCollider();
		}
	}
	// 地面についた
	else {
		// 初めなら攻撃を出し音を出す
		if (activeAttackFrames == 0) {
			SoundManager::GetIns().Play("StampLand");
			collOperator->CollOn();
			collOperator->EffectPlay();
		}
		// 持続時間過ぎたらステートを変える
		if (activeAttackFrames > ATTACK_DURATION) {
			DefaultChangeState();
			return;
		}
		// 時間を経たせる
		activeAttackFrames++;
	}

	// 移動に掛ける時間まで
	if (horizontalMoveFrames < TIME_RATE) {

		// 押し出されるため毎回とる
		attackPos = collOperator->GetAttackPos();
		// 現在の自身と攻撃地点の距離から進む距離を出す
		horizontalMoveStep = Vector3::XZonly(attackPos.x - pos.x, attackPos.z - pos.z) / (TIME_RATE - (float)horizontalMoveFrames);

		// 現在の時間を足す
		horizontalMoveFrames++;
		// 位置を足す
		pos += horizontalMoveStep;

		// 予測線の更新
		float scale = 1.0f - (TIME_RATE - (float)horizontalMoveFrames) / TIME_RATE;
		collOperator->SetScale(scale);
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackB, attackPos, scale));
	}
}

void TomatoBossStampState::Exit(void)
{
	onCollider();
	collOperator->Off();
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB, false));
}

void TomatoBossStampState::AlwaysUpdate(void)
{
}
