#include "CharaSelectOperator.h"

#include "../../Manager/Net/NetWorkManager.h"
#include "../../Manager/Input/KeyManager.h"

#include "CharaSelectPreview/Orange/OrangeCharaSelectPreview.h"

CharaSelectOperator::CharaSelectOperator(MSG_SENDER_ID operatorSenderId) :
	operatorSenderId(operatorSenderId),

	charaPreview(nullptr, nullptr, nullptr),
	selectCharaType(CHARA_TYPE::None)
{
}

void CharaSelectOperator::Load(void)
{
	// プレビュー生成ラムダ関数
	auto CharaPreviewCreate = [&](CHARA_TYPE type)->CharaSelectPreviewBase* {
		switch (type) {
		case CHARA_TYPE::Orange: { return new OrangeCharaSelectPreview(operatorSenderId, CHARA_SELECT_PREVIEW_POS[(int)operatorSenderId]); }
		case CHARA_TYPE::Tomato: { return new OrangeCharaSelectPreview(operatorSenderId, CHARA_SELECT_PREVIEW_POS[(int)operatorSenderId]); }
		case CHARA_TYPE::Tanaka: { return new OrangeCharaSelectPreview(operatorSenderId, CHARA_SELECT_PREVIEW_POS[(int)operatorSenderId]); }
		default: { return nullptr; }
		}
		};
	// 各プレビューの生成
	for (int type = 0; type < (int)CHARA_TYPE::Max; type++) {
		charaPreview[type] = CharaPreviewCreate((CHARA_TYPE)type);
		charaPreview[type]->Load();
	}
}

void CharaSelectOperator::Init(void)
{
	selectCharaType = (CHARA_TYPE)((int)CHARA_TYPE::None + 1);

	// 各キャラタイププレビューの初期化処理
	for (CharaSelectPreviewBase* cp : charaPreview) { cp->Init(); }
}

void CharaSelectOperator::ReceptionUpdate(void)
{
	// 自分の操作権者の接続がある 場合のみ受信処理行う
	if (!Net::GetIns().GetConnectStatus().IsEntry(operatorSenderId)) { return; }

	// 選択中のキャラタイプのプレビューの受信処理
	charaPreview[(int)selectCharaType]->ReceptionUpdate();
}

void CharaSelectOperator::Update(void)
{
	// 自分の操作権者の接続がある 場合のみ更新処理を行う
	if (!Net::GetIns().GetConnectStatus().IsEntry(operatorSenderId)) { return; }

	// 自分が操作権者 の場合のみ更新処理を行う
	if (Net::GetIns().GetSenderId() != operatorSenderId) { return; }

	// 操作権者による操作によって選択キャラを変える
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		// 1つ前のタイプへ
		selectCharaType = (CHARA_TYPE)((int)selectCharaType - 1);
		// 範囲外だったら逆の端へ
		if (selectCharaType <= CHARA_TYPE::None) { selectCharaType = (CHARA_TYPE)((int)CHARA_TYPE::Max - 1); }
		// 変更したことをプレビュー管理クラスへ伝える
		charaPreview[(int)selectCharaType]->Select();
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		// 1つ次のタイプへ
		selectCharaType = (CHARA_TYPE)((int)selectCharaType + 1);
		// 範囲外だったら逆の端へ
		if (selectCharaType >= CHARA_TYPE::Max) { selectCharaType = (CHARA_TYPE)((int)CHARA_TYPE::None + 1); }
		// 変更したことをプレビュー管理クラスへ伝える
		charaPreview[(int)selectCharaType]->Select();
	}

	// 選択中のキャラタイプのプレビューを更新
	charaPreview[(int)selectCharaType]->Update();
}

void CharaSelectOperator::SendUpdate(void)
{
	// 自分の操作権者の接続がある 場合のみ送信を行う
	if (!Net::GetIns().GetConnectStatus().IsEntry(operatorSenderId)) { return; }
	// 自分が操作権者 または 接続ホスト の場合のみ送信を行う
	if (!Net::GetIns().IsHost() && Net::GetIns().GetSenderId() != operatorSenderId) { return; }

	// 選択中のキャラタイプのプレビューの送信処理
	charaPreview[(int)selectCharaType]->SendUpdate();
}

void CharaSelectOperator::Draw(void)
{
	// 自分の操作権者の接続がある 場合のみ描画を行う
	if (!Net::GetIns().GetConnectStatus().IsEntry(operatorSenderId)) { return; }

	// 選択中のキャラタイプのプレビューの描画処理
	charaPreview[(int)selectCharaType]->Draw();
}

void CharaSelectOperator::AlphaDraw(void)
{
	// 自分の操作権者の接続がある 場合のみアルファ描画を行う
	if (!Net::GetIns().GetConnectStatus().IsEntry(operatorSenderId)) { return; }

	// 選択中のキャラタイプのプレビューのアルファ描画処理
	charaPreview[(int)selectCharaType]->AlphaDraw();
}

void CharaSelectOperator::UiDraw(void)
{
	// 自分の操作権者の接続がある 場合のみUI描画を行う
	if (!Net::GetIns().GetConnectStatus().IsEntry(operatorSenderId)) { return; }

	// 選択中のキャラタイプのプレビューのUI描画処理
	charaPreview[(int)selectCharaType]->UiDraw();
}

void CharaSelectOperator::Release(void)
{
	// 各キャラタイププレビューの削除
	for (CharaSelectPreviewBase* cp : charaPreview) {
		if (cp == nullptr) { continue; }
		cp->Release();
		delete cp;
		cp = nullptr;
	}
}