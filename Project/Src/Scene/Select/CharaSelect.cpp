#include "CharaSelect.h"
		 
#include <DxLib.h>
#include "../../Utility/Utility.h"
		 
#include "../../Application/Application.h"
		 
#include "../../Manager/Input/KeyManager.h"
#include "../../Manager/Camera/Camera.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"
#include "../../Manager/Net/NetWorkManager.h"

#include "../SceneManager/SceneManager.h"
		 
#include "../Title/End/EndScene.h"

#include "../../Object/CharaSelect/CharaSelectOperator.h"

CharaSelect::CharaSelect() :
	charaSelectOperator(nullptr, nullptr, nullptr, nullptr)
{
}

void CharaSelect::Load(void)
{
	// ‰¹º‚ÌƒV[ƒ“Ø‚è‘Ö‚¦
	Snd::GetIns().ChangeScene("Title");

	Key::GetIns().SetMouseFixed(false);

	// ƒLƒƒƒ‰‘I‘ğ‘€ìŠÇ—ƒNƒ‰ƒX‚Ì¶¬
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		charaSelectOperator[id] = new CharaSelectOperator((MSG_SENDER_ID)id);
		charaSelectOperator[id]->Load();
	}
}
void CharaSelect::Init(void)
{
	// ƒJƒƒ‰‚Ì‰Šú‰»
	Camera::GetIns().ChangeModeFixedPoint(Vector3(0, 150, -350), Vector3());

	// Ú‘±ó‘Ô‰Šú‰»
	Net::GetIns().Disconnected();

	// ƒLƒƒƒ‰‘I‘ğ‘€ìŠÇ—ƒNƒ‰ƒX‚Ì‰Šú‰»
	for (CharaSelectOperator* ptr : charaSelectOperator) { ptr->Init(); }
}
void CharaSelect::Update(void)
{
	// Ú‘±ƒ^ƒCƒv–¢‘I‘ğ
	if (Net::GetIns().GetState() == NetState::None) {

		// ƒzƒXƒg‚Æ‚µ‚ÄÚ‘±ŠJn
		if (Key::GetIns().GetInfo(KEY_TYPE::DEBUG_HOST_START).down) {
			if (Net::GetIns().StartHost()) { printfDx("ƒzƒXƒg‚Æ‚µ‚ÄÚ‘±‘Ò‚¿"); }
			return;
		}

		// ƒNƒ‰ƒCƒAƒ“ƒg‚Æ‚µ‚ÄÚ‘±ŠJn
		if (Key::GetIns().GetInfo(KEY_TYPE::DEBUG_CLIENT_START).down) {
			Net::GetIns().ConnectClient();
			printfDx("ƒNƒ‰ƒCƒAƒ“ƒg‚Æ‚µ‚ÄÚ‘±");
			return;
		}

		// ƒQ[ƒ€I—¹ˆ—
		if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {
			Snd::GetIns().Play("SystemSelect");
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::TITLE);
			return;
		}

		return;
	}

	// ƒzƒXƒg````````````````````````````````````````````````
	if (Net::GetIns().IsHost()) {
		// ƒLƒƒƒ“ƒZƒ‹
		if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) { Net::GetIns().CloseReceptionToCancel(); clsDx(); return; }

		// ƒV[ƒ“isˆ—
		if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
			// Œø‰Ê‰¹
			Snd::GetIns().Play("SystemButton");
			// ƒV[ƒ“‘JˆÚ
			Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::ChangeSceneGame);
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GAME);

			return;
		}
	}
	// ƒNƒ‰ƒCƒAƒ“ƒg`````````````````````````````````````````````
	else {
		// ƒLƒƒƒ“ƒZƒ‹
		if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) { Net::GetIns().ConnectClientCancel(); clsDx(); return; }

		// ƒVƒXƒeƒ€’Ê’m‚ğóM
		while (MsgDataSystemInform* dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>()) {
			// ƒQ[ƒ€ƒV[ƒ“‚Ö‘JˆÚ‚ğóM
			if (dataPtr->inform == MsgDataSystemInform::INFORM_TYPE::ChangeSceneGame) {
				// Œø‰Ê‰¹
				Snd::GetIns().Play("SystemButton");
				// ƒV[ƒ“‘JˆÚ
				SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GAME);
			}
			delete dataPtr;
		}
	}

	// ‹¤’Ê`````````````````````````````````````````````````

	// ƒLƒƒƒ‰‘I‘ğ‘€ìŠÇ—ƒNƒ‰ƒX‚ÌXV
	for (CharaSelectOperator* ptr : charaSelectOperator) { ptr->ReceptionUpdate(); }
	for (CharaSelectOperator* ptr : charaSelectOperator) { ptr->Update(); }
	for (CharaSelectOperator* ptr : charaSelectOperator) { ptr->SendUpdate(); }
}
void CharaSelect::Draw(void)
{
	DrawFormatStringToHandle(0, App::SCREEN_SIZE_Y - 64, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_64), "ƒLƒƒƒ‰ƒZƒŒƒNƒgƒV[ƒ“");

	// Ú‘±ƒ^ƒCƒv–¢‘I‘ğ
	if (Net::GetIns().GetState() == NetState::None) {
		DrawFormatStringToHandle(0, 50, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_45), "•”‰®‚ğì‚éFu1vƒL[");
		DrawFormatStringToHandle(0, 95, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_45), "•”‰®‚ğ’T‚·Fu2vƒL[");
		return;
	}

	// ƒzƒXƒg````````````````````````````````````````````````
	if (Net::GetIns().IsHost()) {
	}
	// ƒNƒ‰ƒCƒAƒ“ƒg`````````````````````````````````````````````
	else {
	}
	// ‹¤’Ê`````````````````````````````````````````````````

	// ƒLƒƒƒ‰‘I‘ğ‘€ìŠÇ—ƒNƒ‰ƒX‚ÌXV
	for (CharaSelectOperator* ptr : charaSelectOperator) { ptr->Draw(); }
	for (CharaSelectOperator* ptr : charaSelectOperator) { ptr->AlphaDraw(); }
	for (CharaSelectOperator* ptr : charaSelectOperator) { ptr->UiDraw(); }

	DrawFormatStringToHandle(0, 50, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_45), "Ø’fFuEscvƒL[");
	DrawFormatStringToHandle(0, 150, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_45), "‘O‚ÌƒLƒƒƒ‰‚ğ‘I‘ğFu©vƒL[");
	DrawFormatStringToHandle(0, 195, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_45), "Ÿ‚ÌƒLƒƒƒ‰‚ğ‘I‘ğFu¨vƒL[");
	if (Net::GetIns().IsHost()) {
		DrawFormatStringToHandle(0, 240, 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_45), "ƒQ[ƒ€ƒV[ƒ“‚Ö‘JˆÚFuEntervƒL[");
	}
}
void CharaSelect::Release(void)
{
	// ƒLƒƒƒ‰‘I‘ğ‘€ìŠÇ—ƒNƒ‰ƒX‚Ìíœ
	for (CharaSelectOperator* ptr : charaSelectOperator) {
		if (!ptr) { continue; }
		ptr->Release();
		delete ptr;
		ptr = nullptr;
	}

}
