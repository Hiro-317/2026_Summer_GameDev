#include "CharaSelectPreviewBase.h"

#include "../../../Manager/Net/NetWorkManager.h"
#include "../../../Manager/Font/FontManager.h"

#include "../../../Manager/Camera/InstantCamera.h"

#include "../../Common/AnimationController/AnimationController.h""

CharaSelectPreviewBase::CharaSelectPreviewBase() :
	ActorBase(),

	anime(nullptr),
	playAnimeType(0)
{
}

void CharaSelectPreviewBase::Load(void)
{
	modelPreviewScreen = MakeScreen(440, 540, true);

	camera = new InstantCamera();

	SubLoad();
}

void CharaSelectPreviewBase::AnimationControllerCreate(void)
{
	// 生成していなければアニメーションコントローラーを生成
	if (anime == nullptr) { anime = new AnimationController(trans.model); }
}

void CharaSelectPreviewBase::SubInit(void)
{
#pragma region 基底クラスにある機能の挙動設定
	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(false);
	// 衝突時の押し出しを無効にする
	SetPushFlg(false);
#pragma endregion
	if (camera) {
		camera->Init();
		camera->ChangeModeFollow(&trans.pos, Vector3(0, 100, -400), Vector3());
	}
	// アニメーション再生
	if (anime) { anime->Play(playAnimeType); }
}

void CharaSelectPreviewBase::Select(void)
{
	// アニメーションを最初から再生しなおす
	if (anime) {
		anime->Stop();
		anime->Play(playAnimeType);
	}
}

void CharaSelectPreviewBase::Update(void)
{
	if (camera) { camera->Update(); }
	// アニメーション更新
	if (anime) { anime->Update(); }
}

void CharaSelectPreviewBase::Draw(void)
{
	SetDrawScreen(modelPreviewScreen);
	ClearDrawScreen();
	camera->Apply();
	trans.Draw();
	SetDrawScreen(DX_SCREEN_BACK);

	//static Vector2I temp = Vector2I();

	//if (CheckHitKey(KEY_INPUT_T)) { temp.y -= 5; }
	//if (CheckHitKey(KEY_INPUT_G)) { temp.y += 5; }
	//if (CheckHitKey(KEY_INPUT_F)) { temp.x -= 5; }
	//if (CheckHitKey(KEY_INPUT_H)) { temp.x += 5; }

	DrawStringToHandle(340, 120, CHARA_NAME.c_str(), 0xffffff, Font::GetIns().GetFont(Font::FONT_KINDS_SIZE::GOKUSYOU_32));

	DrawFormatStringToHandle(850, 185, 0xffffff, Font::GetIns().GetFont(Font::FONT_KINDS_SIZE::GOKUSYOU_32), "%d", HP);
	DrawFormatStringToHandle(1160, 185, 0xffffff, Font::GetIns().GetFont(Font::FONT_KINDS_SIZE::GOKUSYOU_32), "%d", ATTACK);
	DrawFormatStringToHandle(850, 300, 0xffffff, Font::GetIns().GetFont(Font::FONT_KINDS_SIZE::GOKUSYOU_32), "%d", DEFENCE);
	DrawFormatStringToHandle(1160, 300, 0xffffff, Font::GetIns().GetFont(Font::FONT_KINDS_SIZE::GOKUSYOU_32), "%d", SPEED);

	DrawRotaGraph(400, 455, 1, 0, modelPreviewScreen, true);

	DrawGraph(660, 465, skill_1_image, true);
	DrawGraph(660, 545, skill_2_image, true);
	DrawGraph(660, 630, skill_3_image, true);

	// tempの描画
	//DrawFormatString(0, 750, 0xffffff, "Temp X: %d, Y: %d", temp.x, temp.y);
}

void CharaSelectPreviewBase::SubRelease(void)
{
	if (camera)	{
		camera->Release();
		delete camera;
		camera = nullptr;
	}

	// アニメーションコントローラーを削除
	if (anime) {
		anime->Release();
		delete anime;
		anime = nullptr;
	}

	DeleteGraph(skill_1_image);
	DeleteGraph(skill_2_image);
	DeleteGraph(skill_3_image);
	DeleteGraph(modelPreviewScreen);
}

void CharaSelectPreviewBase::AddInFbxAnimation(int inFbxMaxIndex, float speed, int playAnimeType)
{
	AnimationControllerCreate();
	for (int index = 0; index < inFbxMaxIndex; index++) {
		anime->AddInFbx(index, speed, index);
	}
	this->playAnimeType = playAnimeType;
}

void CharaSelectPreviewBase::AddAnimation(const char* filePath, float speed, int index)
{
	AnimationControllerCreate();
	anime->Add(index, speed, filePath);
	playAnimeType = index;
}