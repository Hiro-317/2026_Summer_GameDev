#pragma once

#include "../../Common/ActorBase/ActorBase.h"

class AnimationController;

class ClearCharaPreviewBase : public ActorBase
{
public:
	ClearCharaPreviewBase(const Vector3& pos, unsigned char operatorNumber);
	virtual ~ClearCharaPreviewBase()override = default;

private:

	// アニメーションコントローラー
	AnimationController* anime;
	void AnimationControllerCreate(void);

	// 主要関数再定義～～～～～～～～～
	void BaseInit(void)override;
	void BaseUpdate(void)override;
	void BaseRelease(void)override;
	// ～～～～～～～～～～～～～～～～

	// 再生するアニメーションのタイプ
	int playAnimeType;

protected:

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