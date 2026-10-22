#include "PlayerBase.h"

#include "../../../Manager/Font/FontManager.h"
#include "../../../Manager/TimeScale/TimeScale.h"
#include "../../../Manager/Net/NetWorkManager.h"

#include "CommonPlayerState/OtherPlayerWatch/OtherPlayerWatchState.h"
#include "CommonPlayerState/Move/PlayerMoveState.h"

#include "../../Common/Collider/LineCollider.h"
#include "../../Common/Collider/CapsuleCollider.h"

#include "../../Common/CharacterBase/CharacterStatsDefine.h"

PlayerBase::PlayerBase(
	MSG_SENDER_ID controlSenderId,

	const std::string fileName,
	const std::string hpParameterID,
	const std::string attackPowerParameterID,
	const std::string defensePowerParameterID,
	const std::string moveSpeedParameterID,
	const std::string& parameterPath,

	std::string modelPath
) :
	CharacterBase(
		fileName,
		hpParameterID,
		attackPowerParameterID,
		defensePowerParameterID,
		moveSpeedParameterID,
		parameterPath
	),
	otherPlayerTrans(),
	bossPos(nullptr),

	targetPlayerIndex(),
	targetPlayerPos(),

	invincibleCounter(),

	isDeath()
{
	trans.LoadModel(("Character/" + modelPath).c_str());

	this->controlSenderId = controlSenderId;
}

void PlayerBase::Load(void)
{
	PlayerLoad();

	if (Net::GetIns().GetConnectStatus().EntryCount() != 1) {
		// 観戦モード
		AddState(
			STATE::OTHER_WATCH,
			new OtherPlayerWatchState(otherPlayerTrans, bossPos)
		);
	}

#pragma region モデル

	// モデルのスケールを設定
	trans.scale = GetParameterToVector3("Model", "Scale");

	// モデルの中心点のズレ
	trans.centerDiff = GetParameterToVector3("Model", "CenterDiff") * trans.scale;

	// 角度のズレ(ローカル回転)
	trans.localRotation =
		Quaternion::FromEulerXZY(
			Deg2Rad(GetParameterToVector3("Model", "LocalRot"))
		);

#pragma endregion

#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 衝突時の押し出しを有効にする
	SetPushFlg(true);

	// 押し出す力の大きさを設定する
	SetPushWeight((unsigned char)GetParameterToInt("Collider", "CollisionPushWeight"));

#pragma endregion

#pragma region 当たり判定情報設定

	// 当たり判定を生成する（線分コライダー）
	AddCollider(new LineCollider(COLLIDER_TAG::Player, Vector3(), -Vector3::Yonly(MODEL_SIZE.y * 0.5f)));

	// 当たり判定を生成する（カプセルコライダー）
	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::Player,
			Vector3::Yonly(
				(MODEL_SIZE.y * 0.5f) * GetParameter("Collider", "ModelToColliderRate") -
				(MODEL_SIZE.x * 0.5f) * GetParameter("Collider", "ModelToColliderRate")
			),
			-Vector3::Yonly(
				(MODEL_SIZE.y * 0.5f) * GetParameter("Collider", "ModelToColliderRate") -
				(MODEL_SIZE.x * 0.5f) * GetParameter("Collider", "ModelToColliderRate") -
				GetParameter("Collider", "ClimbOverHeight")
			),
			(MODEL_SIZE.x * 0.5f) * GetParameter("Collider", "ModelToColliderRate")
		)
	);

#pragma endregion
}

void PlayerBase::SubInit(void)
{
	// 位置を初期位置にする
	trans.pos = GetParameterToVector3("Init", "Pos") + Vector3::Xonly(150.0f) * (float)controlSenderId;

	// 初期状態を移動状態にする
	ChangeState(STATE::IDLE);

	TargetPlayerNext();
}

void PlayerBase::SubUpdate(void)
{
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerToPlayerTarget).down) {
		TargetPlayerNext();
	}

	// HPがゼロ以下になったら死亡状態に遷移
	if (characterStats.hp <= 0 && GetState() != (int)STATE::DEATH) {
		ChangeState(STATE::DEATH);
	}

	// 無敵時間更新
	if (invincibleCounter > 0.0f) {
		invincibleCounter -= TimeScale::Get();
		if (invincibleCounter < 0.0f) { invincibleCounter = 0.0f; }
	}
}

void PlayerBase::ChangeState(int nextState)
{
	// 状態遷移
	CharacterBase::ChangeState(nextState);

	// 遷移するステート(状態)を送信
	if (IsOwnControl()) { Net::GetIns().Send(MsgDataPlayerState(nextState)); }
}

void PlayerBase::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	// 判定そのものはホストがすべて請け負う
	if (!Net::GetIns().IsHost()) { return; }

	// 死亡状態の場合何もしない
	if (GetState() == (int)STATE::DEATH) { return; }

	// 衝突物のコライダータグを取得
	COLLIDER_TAG otherColliderTag = other.GetTag();

#pragma region プラス効果の判定（無敵判定を無視して発動）

	switch (otherColliderTag) {
	case COLLIDER_TAG::PlayerHeal: {	// 回復

		// 回復量を取得
		short heal = other.GetSkillStats()->Power();

		// 回復をクライアントへ送信
		Net::GetIns().Send(MsgDataPlayerHeal(heal), controlSenderId);

		// 回復させる
		characterStats.HpHeal(heal);
		return;
	}
	case COLLIDER_TAG::PlayerBuff: {	// バフ

		// バフデータ構造体を取得
		const ModifierData& modifier = other.GetSkillStats()->ModifierPower();

		// バフをクライアントへ送信
		Net::GetIns().Send(MsgDataPlayerModifier(modifier), controlSenderId);
		
		// バフをかける
		characterStats.AddModifier(modifier);
		return;
	}
	}

#pragma endregion

	// 無敵時間の判定
	if (invincibleCounter > 0.0f) { return; }

#pragma region マイナス効果の判定

	switch (otherColliderTag) {
	case COLLIDER_TAG::BossAttack: {	// 被ダメージ

		// ダメージ状態に遷移
		ChangeState(STATE::DAMAGE);

		// ボスの攻撃力とプレイヤーの防御力で、最終的なダメージ値を計算
		const short damage = CalculateDamage(other.GetSkillStats()->Power(), characterStats.defensePower.Value());

		// プレイヤーが受けるダメージ値を、クライアント側に送信
		Net::GetIns().Send(MsgDataPlayerDamage(damage), controlSenderId);

		// ダメージ値分HPを減らす
		characterStats.hp -= damage;

		// HPがなくなったら死亡状態に遷移させる
		if (characterStats.hp <= 0) { ChangeState(STATE::DEATH); }

		return;
	}
	}

#pragma endregion
}

void PlayerBase::AnimePlay(int type, signed char loop)
{
	// 自身の操作者プレイヤーの更新により、呼び出された再生の場合、
	// 自身のPC以外のすべてに再生したことを送信する
	if (IsOwnControl()) { Net::GetIns().Send(MsgDataPlayerAnimeType(type, loop)); }

	CharacterBase::AnimePlay(type, loop);
}

void PlayerBase::TargetPlayerNext(void)
{
	// ターゲット選択処理
	targetPlayerIndex++;

	// Indexがプレイヤー人数を超過した場合、0に戻す
	if (targetPlayerIndex >= Net::GetIns().GetConnectStatus().EntryCount()) {
		targetPlayerIndex = 0;
	}

	// ターゲットが自分なら、自分の座標を入れる。
	// 他プレイヤーがターゲットなら、そのプレイヤーの座標を代入
	if (targetPlayerIndex == 0) { targetPlayerPos = &trans.pos; }
	else {
		if (otherPlayerTrans.size() >= targetPlayerIndex) {
			targetPlayerPos = &otherPlayerTrans.at(targetPlayerIndex - 1)->pos;
		}
	}

	if (!Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataPlayerToPlayerTarget((unsigned char)targetPlayerIndex), controlSenderId);
	}
}

void PlayerBase::ReceptionUpdate(void)
{
	// 座標・角度の同期
	while (MsgDataPlayerTrans* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerTrans>(controlSenderId)) {
		// 自分のキャラ（操作対象）の場合
		if (IsOwnControl()) {
			// ホストから送られた座標と今の自分の座標の距離を測る
			float diff = (trans.pos - dataPtr->pos).Length();

			// 誤差が小さいなら無視
			if (diff > 0.5f) {
				// 誤差が大きい場合、少しずつホストから送られた座標に寄せる（補間）
				trans.pos = trans.pos * 0.9f + dataPtr->pos * 0.1f;
			}

			// ホストから送られた角度と今の自分の角度のずれを測る
			diff = Quaternion::Angle(trans.rotation, dataPtr->rotation);

			// 誤差が小さいなら無視
			if (diff > 0.5f) {
				// 誤差が大きい場合、少しずつホストから送られた角度を寄せる（補間）
				trans.rotation = Quaternion::Slerp(trans.rotation, dataPtr->rotation, 0.1f);
			}

		}
		// 他人のキャラなら、そのまま同期
		else { 
			trans.pos = dataPtr->pos;
			trans.rotation = dataPtr->rotation;
		}


		delete dataPtr;
	}

	// アニメーション
	while (MsgDataPlayerAnimeType* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerAnimeType>(controlSenderId, true)) {
		AnimePlay(dataPtr->animeType, dataPtr->loop);
		delete dataPtr;
	}

	// 攻撃を受けた時のダメージ
	while (MsgDataPlayerDamage* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerDamage>(controlSenderId)) {
		characterStats.hp -= dataPtr->damage;
		ChangeState(STATE::DAMAGE);

		delete dataPtr;
	}

	// HP
	while (MsgDataPlayerHp* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerHp>(controlSenderId)) {
		characterStats.hp = dataPtr->hp;
		delete dataPtr;
	}

	// 回復
	while (MsgDataPlayerHeal* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerHeal>(controlSenderId)) {
		characterStats.HpHeal(dataPtr->heal);
		delete dataPtr;
	}

	// バフ
	while (MsgDataPlayerModifier* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerModifier>(controlSenderId)) {
		characterStats.AddModifier(dataPtr->modifier);
		delete dataPtr;
	}

	// 味方ターゲット変更
	while (MsgDataPlayerToPlayerTarget* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerToPlayerTarget>(controlSenderId)) {

		targetPlayerIndex = dataPtr->targetIndex;

		// ターゲットが自分なら、自分の座標を入れる。
		// 他プレイヤーがターゲットなら、そのプレイヤーの座標を代入
		if (targetPlayerIndex == 0) { targetPlayerPos = &trans.pos; }
		else {
			if (otherPlayerTrans.size() >= targetPlayerIndex) {
				targetPlayerPos = &otherPlayerTrans.at(targetPlayerIndex - 1)->pos;
			}
		}

		delete dataPtr;
	}

	ActorBase::ReceptionUpdate();
}

void PlayerBase::SendUpdate(void)
{
	if (Net::GetIns().IsHost() || IsOwnControl()) {
		// 自身の座標と角度を
		Net::GetIns().Send(MsgDataPlayerTrans(trans.pos, trans.rotation), controlSenderId);
	} 

	ActorBase::SendUpdate();
}