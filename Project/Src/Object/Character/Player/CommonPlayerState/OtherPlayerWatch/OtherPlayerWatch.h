#pragma once

#include "../../../../../Manager/Camera/Camera.h"
#include "../../../../../Manager/Net/NetWorkManager.h"

#include "../../../CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"

class OtherPlayerWatch : public CharacterStateBase
{
public:
	OtherPlayerWatch(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		std::vector<Vector3>& pos,
		const std::function<void(void)> CameraTargetChange
	);

	~OtherPlayerWatch()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

	const int PLAYER_MAX = Net::GetIns().GetConnectStatus().EntryCount();

	void ChangeCameraTarget() {
	}

	// 座標の参照
	std::vector<Vector3>& pos;

	// カメラの注視点を変える
	const std::function<void(void)> CameraTargetChange;

};

