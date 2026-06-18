#include "CharaSelectPreviewManager.h"

#include "../../Application/Application.h"

#include "../../Manager/Net/NetWorkManager.h"
#include "../../Manager/Input/KeyManager.h"

#include "CharaSelectPreview/Orange/OrangeCharaSelectPreview.h"
#include "CharaSelectPreview/Unknow/UnknowCharaSelectPreview.h"

CharaSelectPreviewManager::CharaSelectPreviewManager() :
	charaPreview(nullptr, nullptr),
	selectCharaType(CHARA_TYPE::None),

	frameImage(-1), arrowImage(-1),
	enterImage(-1, -1),


	easingCounter(0.0f), easingRate(0.0f)
{
}

void CharaSelectPreviewManager::Load(void)
{
	frameImage = LoadGraph("Data/Image/Lobby/CharaSelect/CharaSelectFrame.png");
	arrowImage = LoadGraph("Data/Image/Lobby/CharaSelect/CharaSelectArrow.png");

	enterImage[(int)true] = LoadGraph("Data/Image/Lobby/CharaSelect/CharaSelectEnterController.png");
	enterImage[(int)false] = LoadGraph("Data/Image/Lobby/CharaSelect/CharaSelectEnterKeyboard.png");

	// プレビュー生成ラムダ関数
	auto CharaPreviewCreate = [&](CHARA_TYPE type)->CharaSelectPreviewBase* {
		switch (type) {
		case CHARA_TYPE::Orange: { return new OrangeCharaSelectPreview(); }
		case CHARA_TYPE::Tomato: { return new UnknowCharaSelectPreview(); }
		default: { return nullptr; }
		}
		};
	// 各プレビューの生成
	for (int type = 0; type < (int)CHARA_TYPE::Max; type++) {
		charaPreview[type] = CharaPreviewCreate((CHARA_TYPE)type);
		charaPreview[type]->Load();
	}
}

void CharaSelectPreviewManager::Init(void)
{
	selectCharaType = (CHARA_TYPE)((int)CHARA_TYPE::None + 1);

	// 各キャラタイププレビューの初期化処理
	for (CharaSelectPreviewBase* cp : charaPreview) { cp->Init(); }

	easingCounter = easingRate = 0.0f;
}

void CharaSelectPreviewManager::Update(void)
{
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

	if (easingCounter > 100000.0f) { easingCounter = 0.0f; }

#pragma region イージング
	easingCounter += 0.08f;
	if (easingCounter > 100000.0f) { easingCounter = 0.0f; }
	easingRate += sinf(easingCounter) * 0.001f;
#pragma endregion
}

void CharaSelectPreviewManager::Draw(void)
{
	// 枠の描画
	DrawRotaGraph(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF, 1, 0, frameImage, true);

	// 選択中のキャラタイプのプレビューの描画処理
	charaPreview[(int)selectCharaType]->Draw();

	// 矢印の描画
	DrawRotaGraph(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF, 1 + easingRate, 0, arrowImage, true);

	// 決定キーの描画
	DrawRotaGraph(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y - 48, 1 + easingRate, 0, enterImage[(int)Key::GetIns().LastInputKinds()], true);

}

void CharaSelectPreviewManager::Release(void)
{
	// 各キャラタイププレビューの削除
	for (CharaSelectPreviewBase* cp : charaPreview) {
		if (cp == nullptr) { continue; }
		cp->Release();
		delete cp;
		cp = nullptr;
	}

	for (int& image : enterImage) { DeleteGraph(image); }

	DeleteGraph(arrowImage);
	DeleteGraph(frameImage);
}