#include "BossBase.h"

#include "../../../Manager/Net/NetWorkManager.h"
#include "../../../Manager/Font/FontManager.h"
#include "../../../Manager/Sound/SoundManager.h"
#include "../../../Manager/TimeScale/TimeScale.h"

#include "../../../Scene/SceneManager.h"
#include "../../../Scene/SceneBase.h"

#include "../../Common/Shader/ModelDamageFlashShader.h"

#include "../../../Scene/Common/PostEffect/ScreenShakePostEffect/ScreenShakePostEffect.h"

#include "../../Common/DataLoad/ParameterLoad.h"
#include "../../Common/Shader/ToonShader.h"

#include "../UI/HitUI.h"

#include "../../Common/Collider/XZCircleCollider.h"
#include "../../Common/Collider/LineCollider.h"

#include "../../Common/CharacterBase/CharacterStatsDefine.h"

BossBase::BossBase(const std::string& parameterPath, std::string modelPath) :
	CharacterBase(
		"Parameter",
		"HP_" + std::to_string(Net::GetIns().GetConnectStatus().EntryCount()),
		"AttackPower",
		"DefensePower",
		"MoveSpeed",
		parameterPath
	),

	playerPos(),
	playerIsDeath()
{
	trans.LoadModel(("Character/" + modelPath).c_str());

	this->controlSenderId = Net::HOST_SENDER_ID;

	collParam = new ParameterLoad("Data/Parameter/AttackRange/");

	targetNum = 0;
}

void BossBase::Load(void)
{
#pragma region モデル
	// モデルを読み込む
	BossLoad();

	// モデルのスケールを設定
	trans.scale = MODEL_SCALE;

	// モデルの中心点のズレ
	trans.centerDiff = MODEL_CENTER_DIFF;

	// 角度のズレ(ローカル回転)
	trans.localRotation = MODEL_LOCAL_ROT;

#pragma endregion

#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 衝突時の押し出しを有効にする
	SetPushFlg(true);

	// 押し出す力の大きさを設定する
	SetPushWeight(COLLISION_PUSH_WEIGHT);

#pragma endregion

#pragma region 当たり判定情報設定

	// 当たり判定を生成する（XZ平面上円コライダー）
	if (IsParameterExist("Collider", "ToPlayerDistanceRadius") && IsParameterExist("Collider", "ToPlayerDistanceHeight")) {
		AddCollider(
			new XZCircleCollider(
				COLLIDER_TAG::BossDistance,
				GetParameter("Collider", "ToPlayerDistanceRadius"),
				GetParameter("Collider", "ToPlayerDistanceHeight")
			)
		);
	}

	// 当たり判定を生成する（線分コライダー）
	if (IsParameterExist("Collider", "LineColliderStartPos") && IsParameterExist("Collider", "LineColliderEndPos")) {
		AddCollider(
			new LineCollider(
				COLLIDER_TAG::Boss,
				GetParameterToVector3("Collider", "LineColliderStartPos") * MODEL_SCALE,
				GetParameterToVector3("Collider", "LineColliderEndPos") * MODEL_SCALE
			)
		);
	}

#pragma endregion
}

void BossBase::SubInit(void)
{
	// 位置を初期位置にする
	trans.pos = INIT_POS;
	trans.rotation = Quaternion::FromEulerXZY(INIT_ROT);

	// 初期状態を移動状態にする
	ChangeState(static_cast<int>(STATE::IDLE));

	// 派生それぞれの初期化
	BossInit();
}

void BossBase::SubUpdate(void)
{
#pragma region ターゲット更新

	// ターゲットの生存確認
	if (*playerIsDeath[targetNum]) { targetNum = -1; }

	// 与ダメの最大値を探す
	for (int id = 0; id < Net::GetIns().GetConnectStatus().EntryCount(); id++) {

		// 生存確認
		if (!*playerIsDeath[id]) { continue; }

		// 今の最大ダメージを超えたダメージ蓄積をしたらターゲットを変える
		if (targetNum == -1 || damaged[targetNum] < damaged[id]) {

			// 新しいターゲットを保持
			targetNum = id;

			// ターゲット変更を送信
			Net::GetIns().Send(MsgDataBossTarget((unsigned char)targetNum));
		}
	}

#pragma endregion


#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_0)) {
		short damage = 10;
		characterStats.hp -= damage;
		// プレイヤーが受けるダメージ値を、クライアント側に送信
		Net::GetIns().Send(MsgDataPlayerDamage(damage), controlSenderId);
	}
#endif // _DEBUG
}

void BossBase::SubRelease(void)
{
	collParam->Release();
	delete collParam;
	collParam = nullptr;
}

void BossBase::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (!Net::GetIns().IsHost() || GetState() == (int)STATE::DEATH) return;

	// ボスとプレイヤーの攻撃との当たり判定
	if (ownTag == COLLIDER_TAG::BossDistance) {

		if (other.GetTag() == COLLIDER_TAG::PlayerAttack) {

			// クリティカルフラグ取得
			bool isClitical = false;

			// 防御力も含めたダメージを取得
			short damage = CalculateDamage(other.GetSkillStats()->Power(&isClitical), characterStats.defensePower.Value());

			// ダメージ減らす
			characterStats.hp -= damage;

			// ダメージ情報を送信する
			Net::GetIns().Send(MsgDataBossHit(damage, isClitical, other.GetActor()->GetControlSenderId(), result.point));

			// アイドルだとアニメーションの再生
			if (GetState() == (int)STATE::IDLE) { PlayDamage(); }

			// このPCの操作権プレイヤーによるダメージなら演出
			if (other.GetActor()->GetControlSenderId() == Net::GetIns().GetSenderId()) {

				// ダメージ数値のUIを描画する
				hitUiRef->DamageSetting(result.point, damage, isClitical);

				// クリティカルなら揺らしクリティカル音を出す
				if (isClitical) {
					SceneManager::GetIns().GetScene()->AddPostEffect(new ScreenShakePostEffect());
					Snd::GetIns().Play("CriticalDamaged");
				}
				// 違うなら普通にダメージ音
				else {
					Snd::GetIns().Play("Damaged");
				}

				// ヒットストップ
				TimeScale::GetIns().HitStop(4);
			}

			// HPがゼロ以下になったら死亡状態に遷移
			if (characterStats.hp <= 0 && GetState() != (int)STATE::DEATH) {
				ChangeState(STATE::DEATH);
				return;
			}

			// 与ダメを足す
			damaged[static_cast<int>(other.GetActor()->GetControlSenderId())] += damage;

			// 点滅
			CreateShader(new ModelDamageFlashShader(24, 3, 0.85f, GetVertexType()));
		}
	}
}

void BossBase::ChangeState(int nextState)
{
	// ホストだったら操作者PC以外に伝達する
	if (IsOwnControl()) {
		// 情報を送る
		Net::GetIns().Send(MsgDataBossChangeState(nextState));
	}

	CharacterBase::ChangeState(nextState);
}

void BossBase::AnimePlay(int type, signed char loop)
{
	// ホストだったら操作者PC以外に伝達する
	if (IsOwnControl()) {
		// 情報を送る
		Net::GetIns().Send(MsgDataBossAnimeType(type, loop));
	}

	CharacterBase::AnimePlay(type, loop);
}

void BossBase::ReceptionUpdate(void)
{
	// 座標/角度の同期
	while (MsgDataBossTrans* dataPtr = Net::GetIns().GetMsgData<MsgDataBossTrans>()) {

		// 座標/回転/スケーリング/を同期
		trans.pos = dataPtr->pos;
		trans.rotation = dataPtr->rotation;
		trans.scale = dataPtr->scale;

		delete dataPtr;
	}

	// アニメーション
	while (MsgDataBossAnimeType* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAnimeType>()) {

		// 再生
		AnimePlay(dataPtr->animeType, dataPtr->loop);

		delete dataPtr;
	}

	// ダメージ受信
	while (MsgDataBossHit* dataPtr = Net::GetIns().GetMsgData<MsgDataBossHit>()) {

		// HPを減らす
		characterStats.hp -= dataPtr->damage;

		// このPCの操作権プレイヤーによるダメージなら演出
		if (dataPtr->controlSenderId == Net::GetIns().GetSenderId()) {

			// ダメージ数値のUIを描画する
			hitUiRef->DamageSetting(dataPtr->collisionPoint, dataPtr->damage, dataPtr->clitical);

			// クリティカルなら揺らしクリティカル音を出す
			if (dataPtr->clitical) {
				SceneManager::GetIns().GetScene()->AddPostEffect(new ScreenShakePostEffect());
				Snd::GetIns().Play("CriticalDamaged");
			}
			// 違うなら普通にダメージ音
			else {
				Snd::GetIns().Play("Damaged");
			}

			// ヒットストップ
			TimeScale::GetIns().HitStop(4);
		}

		// 与ダメを足す
		damaged[static_cast<int>(dataPtr->controlSenderId)] += dataPtr->damage;

		delete dataPtr;
	}

	// 標的の受信
	while (MsgDataBossTarget* dataPtr = Net::GetIns().GetMsgData<MsgDataBossTarget>()) {

		targetNum = (int)dataPtr->targetNum;

		delete dataPtr;
	}
}

void BossBase::SendUpdate(void)
{
	if (IsOwnControl()) {
		Net::GetIns().Send(MsgDataBossTrans(trans.pos, trans.rotation, trans.scale));
	}
}

void BossBase::DefaultCreateShader(void)
{
	CreateShader(new ToonShader(GetVertexType()));
}