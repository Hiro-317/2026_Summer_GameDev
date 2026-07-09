#include "GrapeBossStampState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"
#include "../../Weapon/Straight/GrapeBossStraight.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossStampState::GrapeBossStampState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	Vector3& pos, Vector3& angle, const bool& isGround,
	BombType* bombType,
	const std::vector<const Vector3*> playerPos,
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
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), angle(angle), isGround(isGround),
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
	SetCoolTime(SetCoolTime)
{
}

void GrapeBossStampState::Enter(void)
{
	target = GetTarget();
	collOperator->SetPos(*playerPos.at(target));
	attackPos = Vector3();
	attackDistRate = Vector3();
	collOperator->SetDrawArea(true);
	nowAttackTime = 0;
	prevPos = pos.y - 0.5f;
	attackCnt = 0;
	isAttack = true;
	SetCoolTime();
	PlayJumpStartAnim();
	first = true;
	cnt = 0;
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
		if (prevPos < pos.y) {
			prevPos = pos.y;
			pos.y += JUMP_POW;
			offCollider();
		}
		else {
			prevPos = pos.y;
			onCollider();
		}
	}
	else {
		// 一回だけ通る
		if (first) {
			// 地上についたのでスタンプ
			PlayStampAnim();
			// 生成数まで回す
			for (int i = 0; i < WeponDuplicateNum[(int)WeaponType::StampBomb]; i++) {

				// 前後左右の制定
				float FB = GetRand(RADIUS);
				float LR = GetRand(RADIUS);

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
				bombType[i].weaponIns->SetStartPos(Vector3(pos.x + posX, HEIGHT, pos.z + posZ));
				bombType[i].weaponIns->SetViewPosCircle();

				// 起動
				bombType[i].live = true;
			}
			// 位置に攻撃を発生
			collOperator->CollSet(true);
		}
		// 二度目は通さないようにフラグを折る
		first = false;
		
		// 攻撃持続時間の加算
		cnt++;

		// 攻撃持続時間の判定
		if (cnt > ATTACK_DURATION) {
			// フレーム位置に攻撃を消去
			collOperator->CollSet(false);
			collOperator->SetDrawArea(false);
			collOperator->SetScale(0.0f);
		}
	}


	if (nowAttackTime < TIME_RATE) {

		attackPos = collOperator->GetAttackPos();
		attackDistRate = Vector3::XZonly(attackPos.x - pos.x, attackPos.z - pos.z) / (TIME_RATE - (float)nowAttackTime);

		nowAttackTime++;
		pos += attackDistRate;

		// 予測線の更新
		float scale = 1.0f - (TIME_RATE - (float)nowAttackTime) / TIME_RATE;
		collOperator->SetScale(scale);
	}
}

void GrapeBossStampState::Exit(void)
{

}

void GrapeBossStampState::AlwaysUpdate(void)
{
}
