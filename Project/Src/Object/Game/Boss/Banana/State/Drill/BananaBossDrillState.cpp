#include "BananaBossDrillState.h"

#include "../../../../../../Scene/SceneManager.h"
#include "../../../../../../Scene/Game/GameScene.h"

#include "Banadrill/BananaBossDrillManager.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"


BananaBossDrillState::BananaBossDrillState(
	Vector3& pos,
	BananaBossDrillManager* drills,
	const Vector3& defaultPos,
	const std::function<void(void)> PlayFly,
	const std::function<bool(void)> Invisible,
	const std::function<void(void)> SwitchInvisible,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
) :
	CharacterStateBase(),
	pos(pos), drills(drills),
	defaultPos(defaultPos),
	PlayFly(PlayFly),
	Invisible(Invisible),
	SwitchInvisible(SwitchInvisible),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void BananaBossDrillState::Enter(void)
{
	PlayFly();
	pos.y += UP;
	if (scene == nullptr) {
		scene = dynamic_cast<GameScene*>(SceneManager::GetIns().GetScene());
	}
	cnt = 0;
}

void BananaBossDrillState::Update(void)
{
	if (!Invisible()) {
		if (cnt == 0) {
			if (scene != nullptr) {
				scene->SetFocus(false);
				scene->FocusLock(false);
			}
		}
		if (cnt < START_CNT) {
			cnt++;
			if (cnt == START_CNT) {
				drills->Start();
			}
		}
		if (drills->GetEnd()) {
			SwitchInvisible();
			scene->FocusLock(true);
			scene->SetFocus(tmpFlg);
		}
	}
	else {
		if (defaultPos.y < pos.y && pos.y < SWITCH_POS) {
			pos.y += UP;
			if (pos.y > SWITCH_POS) {
				SwitchInvisible();
				tmpFlg = scene->GetFocus();
				pos.y = -SWITCH_POS;
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
	pos = defaultPos;
	SetCoolTime();
	cnt = 0;
}

void BananaBossDrillState::AlwaysUpdate(void)
{
}
