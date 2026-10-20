#pragma once

#include "../../pch.h"

#include<functional>

#include"../SceneBase.h"

#include"../SceneManager.h"

#include "../../Manager/Camera/MultifuncCamera/MultifuncCamera.h"

#include"../../Manager/Input/InputManager.h"
#include"../../Manager/Sound/SoundManager.h"

class GameDebugScene : public SceneBase
{
public:
	GameDebugScene(MultifuncCamera& camera, std::function<void(void)>CameraReset, std::function<void(void)>TopUpdate) : SceneBase(),
		camera(camera),
		TopUpdate(std::move(TopUpdate)),
		CameraReset(std::move(CameraReset))
	{
	}
	~GameDebugScene() = default;

private:

#pragma region 主要関数再定義
	// 初期化
	void SubPostInit(void)override { 
		camera.ChangeModeFree(Deg2Rad(2.0f), 10.0f, camera.GetPos(), camera.GetAngle()); 

		Snd::GetIns().AllStop();
	}

	// 更新
	void SubPostUpdate(void)override {

		if (Input::GetIns().GetInfo(KEY_TYPE::DEBUG_MODE_TOPUPDATE).now) { TopUpdate(); Snd::GetIns().PausePlay(); }
		else { Snd::GetIns().AllStop(); }

		camera.Update();

		if (
			Input::GetIns().GetInfo(KEY_TYPE::DEBUG_MODE_SWITCH).down ||
			Input::GetIns().GetInfo(KEY_TYPE::Pause).down
			) {
			SceneManager::GetIns().PopScene();
		}
	}

	// 終了
	void SubPreRelease(void) { CameraReset(); Snd::GetIns().PausePlay(); }
#pragma endregion

	bool IsUseLoadingScreen(void)const { return false; }

	// 当たり判定管理は使用しない
	bool UseCollisionManager(void)const override { return false; }

	MultifuncCamera& camera;

	std::function<void(void)>TopUpdate;
	std::function<void(void)>CameraReset;
};