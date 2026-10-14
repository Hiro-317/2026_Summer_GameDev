#pragma once

#include "../../Common/ActorBase/ActorBase.h"

class AnimationController;

class LobbyCharaPreviewBase : public ActorBase
{
public:
	LobbyCharaPreviewBase(const Vector3& pos, unsigned char operatorNumber);
	virtual ~LobbyCharaPreviewBase()override = default;

	// 読み込み処理再定義
	void Load(void)override;
	// 更新処理再定義
	void Update(void)override;

	// 
	void OwnOperatorDraw(int image);

private:

	// アニメーションコントローラー
	AnimationController* anime;
	void AnimationControllerCreate(void);

	// 主要関数再定義～～～～～～～～～
	void SubInit(void)override;
	void SubDraw(void)override;
	void SubRelease(void)override;
	// ～～～～～～～～～～～～～～～～

	// 再生するアニメーションのタイプ
	int playAnimeType;

	const std::string OPERATOR_IMAGE_PATH;

	// 操作者を示すイメージ
	int operatorImage;

protected:
	virtual void CharacterLoad(void) = 0;

	// 操作者を示すイメージの高さ
	float operatorImagePos;
	// 自分のキャラを示すイメージの相対座標
	Vector3 ownOperatorImagePos[(int)MSG_SENDER_ID::Max] = {
		Vector3(200, 600, 0),
		Vector3(-200, 600, 0),
		Vector3(-200, 600, 0),
		Vector3(200, 600, 0)
	};

	/// <summary>
	/// モデルにくっついてるFBXアニメーションを全部登録する
	/// </summary>
	/// <param name="inFbxMaxIndex">くっついてるアニメーションの数</param>
	/// <param name="speed">再生速度</param>
	/// <param name="loop">ループ再生フラグ配列（指定なしで全てループ再生有効で登録）</param>
	void AddInFbxAnimation(int inFbxMaxIndex, float speed, bool loop, int playAnimeType);

	/// <summary>
	/// 別ファイルに保存されているFBXアニメーションを登録する
	/// </summary>
	/// <param name="index">参照番号</param>
	/// <param name="speed">再生速度</param>
	/// <param name="loop">ループ再生フラグ</param>
	/// <param name="filePath">パス</param>
	void AddAnimation(int index, float speed, bool loop, const char* filePath);
};