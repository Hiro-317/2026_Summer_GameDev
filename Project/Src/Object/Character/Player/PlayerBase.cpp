#include "PlayerBase.h"

#include "../../../Manager/Camera/Camera.h"
#include "../../../Manager/Font/FontManager.h"

#include "CommonPlayerState/Move/PlayerMoveState.h"

#include "../../UI/HitUI/HitUI.h"

#include "../../Common/Collider/LineCollider.h"
#include "../../Common/Collider/CapsuleCollider.h"


PlayerBase::PlayerBase(

	short HP_MAX,
	short ATTACK_POWER,
	short DEFENSE_POWER,
	short SPEED_POWER,

	std::string modelPath
) :
	CharacterBase(
		HP_MAX,
		ATTACK_POWER,
		DEFENSE_POWER,
		SPEED_POWER),
	otherPlayerTrans()
{
	trans.Load(("Charactor/" + modelPath).c_str());
	this->operatorSenderId = operatorSenderId;
	isOwnOperator = operatorSenderId == Net::GetIns().GetSenderId();
}

PlayerBase::PlayerBase(
	MSG_SENDER_ID operatorSenderId,

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
	bossPos(nullptr)
{
	trans.Load(("Charactor/" + modelPath).c_str());

	this->operatorSenderId = operatorSenderId;
	isOwnOperator = operatorSenderId == Net::GetIns().GetSenderId();
}

void PlayerBase::CharacterLoad(void)
{
	PlayerLoad();

#pragma region モデル
	// モデルを読み込む

	// モデルのスケールを設定
	trans.scale = MODEL_SCALE;

	// モデルの中心点のズレ
	trans.centerDiff = MODEL_CENTER_DIFF;

	// 角度のズレ(ローカル回転)
	trans.localAngle = MODEL_LOCAL_ROT;

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

	// 無敵中に赤点滅を表示しない
	SetInviEffectFlg(false);

#pragma endregion

#pragma region 当たり判定情報設定

	// 当たり判定を生成する（線分コライダー）
	ColliderCreate(new LineCollider(COLLIDER_TAG::PLAYER, LINE_COLLIDER_START_POS, LINE_COLLIDER_END_POS));

	// 当たり判定を生成する（カプセルコライダー）
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::PLAYER,
			CAPSULE_COLLIDER_START_POS, CAPSULE_COLLIDER_END_POS,
			CAPSULE_COLLIDER_RADIUS,
			CAPSULE_COLLIDER_ENOUGH_DISTANCE
		)
	);

#pragma endregion

	// まとめて読み込み処理
	for (ActorBase*& c : subObjArray) { c->Load(); }
}

void PlayerBase::CharacterInit(void)
{
	// 位置を初期位置にする
	trans.pos = INIT_POS;

	// 初期状態を移動状態にする
	state = (int)STATE::MOVE;

	for (ActorBase*& c : subObjArray) { c->Init(); }
}

void PlayerBase::CharacterUpdate(void)
{
	interestPos = trans.pos + INTEREST_POS;

	for (ActorBase*& c : subObjArray) { c->Update(); }
	// HPがゼロ以下になったら死亡状態に遷移
	if (characterStats.hp <= 0 && state != (int)STATE::DEATH) {
		ChangeState((int)STATE::DEATH);
		isDeath = true;
	}

#ifdef _DEBUG		// クールタイム用
	if (CheckHitKey(KEY_INPUT_0)) {
		short damage = 10;
		characterStats.hp -= damage;
		// プレイヤーが受けるダメージ値を、クライアント側に送信
		Net::GetIns().Send(MsgDataPlayerDamage(damage), operatorSenderId);
	}
	if (state == (int)STATE::DEATH) {
		// 不動オブジェクトにする
		SetDynamicFlg(false);
	}
#endif // _DEBUG
}

void PlayerBase::CharacterRemoteUpdate(void)
{
	for (ActorBase*& c : subObjArray) { c->Update(); }
	// HPがゼロ以下になったら死亡状態に遷移
	if (characterStats.hp <= 0) { isDeath = true; }
}

void PlayerBase::CharacterDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->Draw(); }
}

void PlayerBase::CharacterAlphaDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->AlphaDraw(); }
}

void PlayerBase::CharacterUiDraw(void)
{
	// デバッグ用描画
	if (App::GetIns().IsDrawDebug()) {
		int number = 0;
		if (isOwnOperator) {
			number = 0;
		}
		else {
			for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
				if (Net::GetIns().GetSenderId() == (MSG_SENDER_ID)id) { continue; }
				number++;
				if (operatorSenderId == (MSG_SENDER_ID)id) { break; }
			}
		}

		// 1行ずつ描画するためのラムダ式（デバッグ用）
		int yPos = 150; int xPos[4] = { 0, 400, 600, 900 };  const int FONT_SIZE = 20;

		auto debugDrwStr = [&](std::string str)->void {
			DrawStringToHandle(xPos[number], yPos, str.c_str(), 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_20));
			yPos += FONT_SIZE;
			};

		// 加速度をデバッグ表示
		debugDrwStr("プレイヤー～～～～～～");
		debugDrwStr("座標" + std::to_string(trans.pos.x) + ", " + std::to_string(trans.pos.y) + ", " + std::to_string(trans.pos.z));
		debugDrwStr("加速度:" + std::to_string(accelSum.Length()));
		debugDrwStr("スタミナ:" + std::to_string(dynamic_cast<PlayerMoveState&>(GetStateIns((int)STATE::MOVE)).GetDashStamina()));
		debugDrwStr("息切れ:" + std::string(dynamic_cast<PlayerMoveState&>(GetStateIns((int)STATE::MOVE)).IsTired() ? "true" : "false"));
		debugDrwStr("HP：" + std::to_string(characterStats.hp));
		debugDrwStr("～～～～～～('#；ω;`)");
	}
}

void PlayerBase::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)
{
	if (!Net::GetIns().IsHost()) { return; }
	if (GetInviCounter() > 0) { return; }

	// 回避中の無敵処理
	if (state == (int)STATE::SKILL_3) {
		switch (other.GetTag()) {
		case COLLIDER_TAG::BOSS_ATTACK:
			// 回避成功時の無敵時間
			SetInviCounter(DODGE_INVI_TIME);

			// ホストが操作者だった場合表示「ミス！」を出現させる
			if (isOwnOperator) { SubUiSerch<HitUI>()->MissSetting(); }
			// ホスト以外が回避した場合、クライアント側に回避した通知を送る
			else { Net::GetIns().Send(MsgDataPlayerMissNotice(operatorSenderId)); } 
			break;
		}
		return;
	}

	if (state == (int)STATE::DEATH) { return; }

	switch (other.GetTag()) {
	case COLLIDER_TAG::BOSS_ATTACK:		// ボスの攻撃
		// ダメージ状態に遷移
		ChangeState((int)STATE::DAMAGE);
		// ボスの攻撃力とプレイヤーの防御力で、最終的なダメージ値を計算
		const short damage = CalculateDamage(other.GetSkillStats().Power(), characterStats.defensePower.Value());
		// プレイヤーが受けるダメージ値を、クライアント側に送信
		Net::GetIns().Send(MsgDataPlayerDamage(damage), operatorSenderId);
		// ダメージ値分HPを減らす
		characterStats.hp -= damage;
		break;

	case COLLIDER_TAG::STAGE:
		if (other.GetShape() == ColliderBase::SHAPE::BOX) {

			shadowPos.emplace(ownTag, trans.pos);
		}
	}
}

void PlayerBase::CharacterRelease(void)
{
	for (ActorBase*& c : subObjArray) {
		if (c) {
			c->Release();
			delete c;
			c = nullptr;
		}
	}
	subObjArray.clear();
}

void PlayerBase::ChangeState(int state)
{
	CharacterBase::ChangeState(state);

	if (isOwnOperator) { Net::GetIns().Send(MsgDataPlayerState(state)); }

	// ホストだったら操作者PC以外に伝達する
	if (Net::GetIns().IsHost()) {

		// すべてのIDを精査する
		for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
			// そのIDが未参加だったらスキップ(それ以降もないため「break」)
			if (Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }

			// ホストには送らない(ここを通るのがホストであるため自分には送らない)
			if (id == (int)Net::GetIns().GetSenderId()) { continue; }

			// また、この情報の発信源である送信者IDを持つPCにも送らない
			if (id == (int)operatorSenderId) { continue; }

			// それ以外のこの情報が伝わってないPCに情報を送る
			Net::GetIns().Send(MsgDataPlayerState(state), operatorSenderId, (MSG_SENDER_ID)id);
		}
	}
}

void PlayerBase::AnimePlay(int type, bool loop)
{
	// 自身の操作者プレイヤーの更新により、呼び出された再生の場合、
	// 自身のPC以外のすべてに再生したことを送信する
	if (isOwnOperator) { Net::GetIns().Send(MsgDataPlayerAnimeType(type, loop)); }

	// ホストだったら操作者PC以外に伝達する
	if (Net::GetIns().IsHost()) {

		// すべてのIDを精査する
		for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
			// そのIDが未参加だったらスキップ(それ以降もないため「break」)
			if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }

			// ホストには送らない(ここを通るのがホストであるため自分には送らない)
			if (id == (int)Net::GetIns().GetSenderId()) { continue; }

			// また、この情報の発信源である送信者IDを持つPCにも送らない
			if (id == (int)operatorSenderId) { continue; }

			// それ以外のこの情報が伝わってないPCに情報を送る
			Net::GetIns().Send(MsgDataPlayerAnimeType(type, loop), operatorSenderId, (MSG_SENDER_ID)id);
		}
	}

	CharacterBase::AnimePlay(type, loop);
}
void PlayerBase::ReceptionUpdate(void)
{
	// 座標・角度
	while (MsgDataPlayerTrans* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerTrans>(operatorSenderId)) {
		// 自分のキャラ（操作対象）の場合
		if (isOwnOperator) {
			// ホストから送られた座標と今の自分の座標の距離を測る
			float diff = (trans.pos, dataPtr->pos).Length();

			// 誤差が小さいなら無視
			if (diff > 0.5f) {
				// 誤差が大きい場合、少しずつホストから送られた座標に寄せる（補間）
				trans.pos = trans.pos * 0.9f + dataPtr->pos * 0.1f;
			}
		}
		// 他人のキャラなら、そのまま同期
		else { trans.pos = dataPtr->pos; }

		// 角度を同期
		trans.angle = dataPtr->angle;
		delete dataPtr;
	}

	// アニメーション
	while (MsgDataPlayerAnimeType* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerAnimeType>(operatorSenderId)) {
		AnimePlay(dataPtr->animeType);
		delete dataPtr;
	}

	// 攻撃を受けた時のダメージ
	while (MsgDataPlayerDamage* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerDamage>(operatorSenderId)) {
		characterStats.hp -= dataPtr->damage;
		ChangeState((int)STATE::DAMAGE);

		delete dataPtr;
	}

	// HP
	while (MsgDataPlayerHp* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerHp>(operatorSenderId)) {
		characterStats.hp = dataPtr->hp;
		delete dataPtr;
	}

	// 回避した時の「ミス！」表示
	while (MsgDataPlayerMissNotice* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerMissNotice>(operatorSenderId)) 
	{
		if (Net::GetIns().IsHost()) { break; }
		
		if (dataPtr->playerNo == Net::GetIns().GetSenderId()) {
			SubUiSerch<HitUI>()->MissSetting();
		}
		
		delete dataPtr;
	}
}

void PlayerBase::SendUpdate(void)
{
	if (Net::GetIns().IsHost() || isOwnOperator) {
		Net::GetIns().Send(MsgDataPlayerTrans(trans.pos, trans.angle), operatorSenderId);
	} 
}