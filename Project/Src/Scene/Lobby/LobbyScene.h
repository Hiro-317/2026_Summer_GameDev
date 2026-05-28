#pragma once

#include "../SceneBase.h"

#include "../../Common/Vector2.h"

#include "../../Application/Application.h"

class LobbyScene : public SceneBase
{
public:
	LobbyScene();
	~LobbyScene() = default;

	// 読み込み
	void Load(void)override;
	// 初期化処理
	void Init(void)override;
	// 更新ステップ
	void Update(void)override;
	// 描画処理
	void Draw(void)override;
	// 解放処理
	void Release(void)override;

private:

	enum class CHOICE {
		None = -1,

		Exit,
		CharaChange,
		Enter,

		Max
	};

	// 選択中のボタンのインデックス
	CHOICE choice;

	// 看板の画像
	int boardImage;

	// ボタンの画像
	int choiceButtonImage[(int)CHOICE::Max][2];

	// ボタンの画像の位置
	const Vector2I choiceButtonPos[(int)CHOICE::Max] = {
		{ 150, App::GetIns().SCREEN_SIZE_Y - 150 },									// 戻る
		{ App::GetIns().SCREEN_SIZE_X - 450, App::GetIns().SCREEN_SIZE_Y - 150 },	// キャラチェンジ
		{ App::GetIns().SCREEN_SIZE_X - 150, App::GetIns().SCREEN_SIZE_Y - 150 },	// 決定
	};

	// 選択中のボタンの上に表示する矢印の画像
	int arrowImage;

	// 選択中のボタンの上に表示する決定キーの画像
	int enterKeyImage[2];


};