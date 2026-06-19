#pragma once

#include "../../Manager/Net/NetWorkDefine.h"

#include "../ActorBase.h"
#include "CharaSelectPreview/CharaSelectPreviewBase.h"

#include "../CharaTypeDefine.h"

class CharaSelectPreviewManager : public ActorBase
{
public:
	CharaSelectPreviewManager();
	~CharaSelectPreviewManager()override = default;

	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void AlphaDraw(void)override {}
	void Release(void)override;

	const CHARA_TYPE GetCharaType(void) { return selectCharaType; }
private:

#pragma region 定数定義

	const Vector2I EXIT_IMAGE_POS = Vector2I(174, 77);

#pragma endregion


	// 枠の画像
	int frameImage;

	// 矢印の画像
	int arrowImage;

	// 決定キーの画像
	int enterImage[2];

	// とじるキーの画像
	int exitImage[2];

	// 拡大演出カウンター
	float easingCounter;
	// 拡大演出の最終的な倍率
	float easingRate;

	// 選択中キャラのプレビューオブジェクト配列
	CharaSelectPreviewBase* charaPreview[(int)CHARA_TYPE::Max];

	// 選択中のキャラタイプ
	CHARA_TYPE selectCharaType;
};