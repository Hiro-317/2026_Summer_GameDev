#include "BossSelectScene.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../SceneManager/SceneManager.h"

BossSelectScene::BossSelectScene(const std::function<void(void)>& LobbyPreviewBossChange):
	SceneBase(),
	
	LobbyPreviewBossChange(LobbyPreviewBossChange),

	bossSelectImage(),
	arrowImage(-1),
	enterImage(),
	exitImage(),

	easingCounter(0.0f),
	easingRate(0.0f)
{
}

void BossSelectScene::Load(void)
{
#pragma region 画像の読み込み
	// ボス選択画像
	for (int i = 0; i < (int)BOSS_TYPE::Max; i++) { bossSelectImage[i] = LoadGraph(BOSS_SELECT_IMAGE_PATH[i].c_str()); }

	// 矢印
	arrowImage = LoadGraph(ARROW_IMAGE_PATH.c_str());

	// 決定キー
	enterImage[(int)true] = LoadGraph(CONTROLLER_ENTER_KEY_IMAGE_PATH.c_str());
	enterImage[(int)false] = LoadGraph(KEYBOARD_ENTER_KEY_IMAGE_PATH.c_str());

	// とじるキー
	exitImage[(int)true] = LoadGraph(KEYBOARD_EXIT_KEY_IMAGE_PATH.c_str());
	exitImage[(int)false] = LoadGraph(CONTROLLER_EXIT_KEY_IMAGE_PATH.c_str());
#pragma endregion
}

void BossSelectScene::Init(void)
{
	selectBossType = SceneManager::GetIns().GetSelectBossType();
	if (selectBossType <= BOSS_TYPE::None || BOSS_TYPE::Max <= selectBossType) { selectBossType = (BOSS_TYPE)((int)BOSS_TYPE::None + 1); }

	easingCounter = easingRate = 0.0f;
}

void BossSelectScene::Update(void)
{
	// 戻る
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

		// このシーンを破棄
		SceneManager::GetIns().PopScene();

		// 以降はthisがnullptrとなっているため終了
		return;
	}

	// 操作によって選択キャラを変える
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		// 1つ前のタイプへ
		selectBossType = (BOSS_TYPE)((int)selectBossType - 1);
		// 範囲外だったら逆の端へ
		if (selectBossType <= BOSS_TYPE::None) { selectBossType = (BOSS_TYPE)((int)BOSS_TYPE::Max - 1); }
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		// 1つ次のタイプへ
		selectBossType = (BOSS_TYPE)((int)selectBossType + 1);
		// 範囲外だったら逆の端へ
		if (selectBossType >= BOSS_TYPE::Max) { selectBossType = (BOSS_TYPE)((int)BOSS_TYPE::None + 1); }
	}

	// 決定
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

		if (selectBossType <= BOSS_TYPE::None || BOSS_TYPE::Max <= selectBossType) { return; }

		// 選択中のキャラで、SceneManagerが抱えているキャラ選択情報を書き換える
		SceneManager::GetIns().SetSelectBossType(selectBossType);

		// 情報を送信する
		if (Net::GetIns().IsHost()) { Net::GetIns().Send(MsgDataBossSelect((int)selectBossType)); }

		// ロビーシーンのプレビューを更新する
		LobbyPreviewBossChange();

		// キャラ変更終了でこのシーンを破棄
		SceneManager::GetIns().PopScene();

		// 以降はthisがnullptrとなっているため終了
		return;
	}

#pragma region イージング
	easingCounter += 0.08f;
	if (easingCounter > 100000.0f) { easingCounter = 0.0f; }
	easingRate += sinf(easingCounter) * 0.001f;
#pragma endregion
}

void BossSelectScene::Draw(void)
{
	// 画面全体を半透明の黒で塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 画像描画用ラムダ関数
	auto DrawImageToVector2I = [&](Vector2I pos, int imageHandle, float scale = 1.0f)->void {
		DrawRotaGraph(pos.x, pos.y, scale, 0, imageHandle, true);
		};

	// ボス選択画像の描画
	DrawImageToVector2I(BOSS_SELECT_IMAGE_POS, bossSelectImage[(int)selectBossType]);

	// 矢印の描画
	DrawImageToVector2I(ARROW_IMAGE_POS, arrowImage, 1.0f + easingRate);

	// 決定キーの描画
	DrawImageToVector2I(ENTER_IMAGE_POS, enterImage[(int)Key::GetIns().LastInputKinds()], 1.0f + easingRate);

	// とじるキーの描画
	DrawImageToVector2I(EXIT_IMAGE_POS, exitImage[(int)Key::GetIns().LastInputKinds()], 1.0f + easingRate);
}

void BossSelectScene::Release(void)
{
#pragma region 画像の解放
	// ボス選択画像
	for (int i = 0; i < (int)BOSS_TYPE::Max; i++) { DeleteGraph(bossSelectImage[i]); }

	// 矢印
	DeleteGraph(arrowImage);

	// 決定キー
	DeleteGraph(enterImage[(int)true]);
	DeleteGraph(enterImage[(int)false]);

	// とじるキー
	DeleteGraph(exitImage[(int)true]);
	DeleteGraph(exitImage[(int)false]);
#pragma endregion

}