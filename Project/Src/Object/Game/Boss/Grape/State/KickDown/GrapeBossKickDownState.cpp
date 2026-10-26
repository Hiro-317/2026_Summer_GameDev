#include "GrapeBossKickDownState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

#include "GrapeKickDownCollOperator.h"
#include "../../Weapon/KickBomb/GrapeBossKickBomb.h"

GrapeBossKickDownState::GrapeBossKickDownState(
	Transform& bossTrans,

	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	const int& targetNum,

	const Quaternion& LOCAL_ROT,

	const Vector3& MODEL_SCALE,

	GrapeKickDownCollOperator* collOperator,
	GrapeBossKickBomb* bomb,

	const std::function<void(void)>& playAttackAnim,
	const std::function<float(void)>& getAnimPlayRatio,
	const std::function<void(void)>& defaultChangeState,
	const std::function<void(void)>& setCoolTime
) :
	bossTrans(bossTrans),
	playerPos(playerPos),
	targetNum(targetNum),
	LOCAL_ROT(LOCAL_ROT),
	MODEL_SCALE(MODEL_SCALE),
	collOperator(collOperator),
	bomb(bomb),
	playAttackAnim(playAttackAnim),
	getAnimPlayRatio(getAnimPlayRatio),
	defaultChangeState(defaultChangeState),
	setCoolTime(setCoolTime)
{
}

void GrapeBossKickDownState::Enter(void)
{
	playAttackAnim();

	first = true;
	cnt = 0;

	setCoolTime();

	bossTrans.rotation = Quaternion::FromRotationY(atan2f(playerPos[target]->x - bossTrans.pos.x, playerPos[target]->z - bossTrans.pos.z));

	attackPos = Vector3::XZonly(bossTrans.pos.x, bossTrans.pos.z) + bossTrans.rotation.Rotate(LOCAL_ROT.Rotate(FOOT_VIEW_POS * MODEL_SCALE));
	
	collOperator->SetPos(attackPos);
	collOperator->SetDrawArea(true);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA));
	Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA, attackPos, 0.0f));
}

void GrapeBossKickDownState::Update(void)
{
	// アニメーションが終わっていたらステートを抜ける
	if (getAnimPlayRatio() >= 1.0f) {
		// アニメーションの再生割合で生成
		if (first) {
			// 生成数まで回す
			for (int i = 0; i < WeponDuplicateNum[(int)WeaponType::KickBomb]; i++) {

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
				bomb[i].SetPos(tmpPos);
				bomb[i].SetViewPosCircle();
			}
			// フレーム位置に攻撃と音を発生
			collOperator->CollSet(true);
			Snd::GetIns().Play("Kick");
		}
		// 二度目は通さないようにフラグを折る
		first = false;
		// 攻撃持続時間の加算
		cnt++;

		// 攻撃持続時間の判定
		if (cnt > ATTACK_DURATION) {

			defaultChangeState();
		}
	}
	else {
		// 攻撃描画を更新
		collOperator->SetScale(getAnimPlayRatio());
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA, attackPos, getAnimPlayRatio()));
	}
}

void GrapeBossKickDownState::Exit(void)
{
	// フレーム位置に攻撃を消去
	collOperator->CollSet(false);
	collOperator->SetDrawArea(false);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA, false));
}

void GrapeBossKickDownState::AlwaysUpdate(void)
{
}
