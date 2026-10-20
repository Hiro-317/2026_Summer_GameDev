#pragma once

#include "../../Common/ActorBase/ActorBase.h"

#include "../../Common/Common/CharaTypeDefine.h"

#include <map>

class BossSelectSpotArea;

class BossSelectSpotBase : public ActorBase
{
public:
	BossSelectSpotBase(
		BOSS_TYPE bossType,
		std::string MODEL_PATH,
		float defaultModelScale, float selectedModelScale,
		float DEFAULT_COLLIDER_RADIUS,
		Vector3 DEFAULT_SELECTED_IMAGE_OFFSET,
		const Vector3& pos, const Quaternion& rotation
	);
	~BossSelectSpotBase()override = default;

	void Load(void)override;

	void SelectedDraw(const int& selectedImage);

private:
	
	// モデルパス
	const std::string MODEL_PATH;

	// モデルの表示倍率
	const std::map<bool, float> MODEL_SCALE;

	// モデルの表示倍率の変化速度
	static constexpr float MODEL_SCALE_EASING_RATE = 1.0f / 10.0f;

	// コライダーの半径
	const float COLLIDER_RADIUS;

	// 選択時に表示するイメージの相対座標
	const Vector3 SELECTED_IMAGE_OFFSET;

	// 現在飛行船が近くにいるかどうか（現フレーム）
	bool selected;

	// 選択時表示イメージのイージングカウンター
	float selectedEasingCounter;
	// 選択時表示イメージのイージングレート
	float selectedEasingRate;

	BossSelectSpotArea* area;

	void SubUpdate(void)override;
};