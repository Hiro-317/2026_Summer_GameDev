#include "BananaBossDrillState.h"

#include "../../../../../../Scene/SceneManager.h"
#include "../../../../../../Scene/Game/GameScene.h"

#include "Banadrill/BananaBossDrillManager.h"

#include "../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../Manager/Net/NetWorkManager.h"


BananaBossDrillState::BananaBossDrillState(
	Vector3& pos,
	BananaBossDrillManager* drills,
	const Vector3& defaultPos,
	const std::function<void(void)> PlayFly,
	const std::function<bool(void)> IsVisible,
	const std::function<void(void)> ToggleVisibility,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
) :
	CharacterStateBase(),
	pos(pos), drills(drills),
	defaultPos(defaultPos),
	PlayFly(PlayFly),
	IsVisible(IsVisible),
	ToggleVisibility(ToggleVisibility),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void BananaBossDrillState::Enter(void)
{
	PlayFly();
	pos.y += UP;
	if (gameScene == nullptr) {
		gameScene = dynamic_cast<GameScene*>(SceneManager::GetIns().GetScene());
	}
	shouldRestoreFocus = gameScene != nullptr && gameScene->GetFocus();
	hiddenFrames = 0;
	Snd::GetIns().Play("Fly");
}

void BananaBossDrillState::Update(void)
{
	if (!IsVisible()) {
		if (hiddenFrames == 0) {
			if (gameScene != nullptr) {
				if (shouldRestoreFocus) {
					gameScene->SetFocus(false);
				}
				gameScene->FocusLock(false);
			}
			Snd::GetIns().Stop("Fly");
		}
		if (hiddenFrames < START_CNT) {
			hiddenFrames++;
			if (hiddenFrames == START_CNT) {
				drills->Start();
			}
		}
		if (drills->GetEnd()) {
			ToggleVisibility();
			if (gameScene != nullptr) { gameScene->FocusLock(true); }
			Snd::GetIns().Play("Fly");
			if (shouldRestoreFocus && gameScene != nullptr) {
				gameScene->SetFocus(true);
				shouldRestoreFocus = false;
			}
		}
	}
	else {
		if (defaultPos.y < pos.y && pos.y <= SWITCH_POS) {
			pos.y += UP;
			if (pos.y >= SWITCH_POS) {
				ToggleVisibility();
				pos.y = -SWITCH_POS;
				if (gameScene != nullptr && gameScene->GetFocus()) {
					shouldRestoreFocus = true;
				}
			}
		}
		if (defaultPos.y > pos.y) {
			pos.y += UP;
			if (pos.y >= defaultPos.y) {
				DefaultChangeState();
			}
		}
	}
}

void BananaBossDrillState::Exit(void)
{
	// 死亡で中断されても、非表示・カメラロック解除を引き継がない。
	if (!IsVisible()) { ToggleVisibility(); }
	if (gameScene != nullptr) {
		gameScene->FocusLock(true);
		if (shouldRestoreFocus) { gameScene->SetFocus(true); }
	}
	shouldRestoreFocus = false;
	pos = defaultPos;
	SetCoolTime();
	hiddenFrames = 0;
	Snd::GetIns().Stop("Fly");
}

void BananaBossDrillState::AlwaysUpdate(void)
{
}
