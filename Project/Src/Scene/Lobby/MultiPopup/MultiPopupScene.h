#pragma once

#include "../../SceneBase.h"

#include "../../../Application/Application.h"

class MultiPopupScene : public SceneBase
{
public:
	MultiPopupScene();
	~MultiPopupScene()override = default;

	// “Ç‚İ‚İ
	void Load(void)override;
	// ‰Šú‰»ˆ—
	void Init(void)override;
	// XVƒXƒeƒbƒv
	void Update(void)override;
	// •`‰æˆ—
	void Draw(void)override;
	// ‰ğ•úˆ—
	void Release(void)override;


private:
#pragma region ’è”’è‹`
	
	enum class SELECT {
		None = -1,

		MakeRoom,	// •”‰®‚ğ‚Â‚­‚é
		JoinRoom,	// •”‰®‚ğ‚³‚ª‚·

		Max
	};

	// ‰æ‘œ‚ÌƒpƒX````````````````````````````

	// ƒfƒBƒŒƒNƒgƒŠ
	const std::string DIRECTORY_PATH = "Data/Image/Lobby/";
	
	// ƒ|ƒbƒvƒAƒbƒvƒtƒŒ[ƒ€
	const std::string POPUP_FRAME_PATH = DIRECTORY_PATH + "MultiPopup/PopupFrame.png";

	// ‘I‘ğˆ
	const std::string SELECT_PATH[(int)SELECT::Max] = {
		DIRECTORY_PATH + "MultiPopup/MakeRoom.png",
		DIRECTORY_PATH + "MultiPopup/JoinRoom.png",
	};

	// ‘I‘ğ’†‚ğ¦‚·ƒtƒŒ[ƒ€
	const std::string SELECT_FRAME_PATH = DIRECTORY_PATH + "MultiPopup/SelectFrame.png";

	// ‘I‘ğ’†‚ğ¦‚·–îˆó
	const std::string SELECT_ARROW_PATH = DIRECTORY_PATH + "MultiPopup/SelectArrow.png";

	// ‘I‘ğ’†‚Ìƒ{ƒ^ƒ“‚Ìã‚É•\¦‚·‚éŒˆ’èƒL[
	const std::string CONTROLLER_ENTER_KEY_PATH = DIRECTORY_PATH + "NowSelectController.png";
	const std::string KEYBOARD_ENTER_KEY_PATH = DIRECTORY_PATH + "NowSelectKeyboard.png";

	// ````````````````````````````‰æ‘œ‚ÌƒpƒX

	// Še‰æ‘œ‚Ì•`‰æÀ•W`````````````````````````
	
	// ƒ|ƒbƒvƒAƒbƒvƒtƒŒ[ƒ€
	const Vector2I POPUP_FRAME_POS = Vector2I(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF);

	// ‘I‘ğˆ
	const Vector2I SELECT_POS[(int)SELECT::Max] = {
		Vector2I(510,App::SCREEN_SIZE_Y_HALF + 40),
		Vector2I(930,App::SCREEN_SIZE_Y_HALF + 40),
	};

	// ‘I‘ğ’†‚Ìƒ{ƒ^ƒ“‚Ìã‚É•\¦‚·‚éŒˆ’èƒL[‚Ì‘Š‘ÎÀ•W
	const Vector2I ENTER_KEY_LOCAL_POS = Vector2I(0, 195);
	const float ENTER_KEY_LOCAL_POS_EASING_RATE = 100.0f;

	// `````````````````````````Še‰æ‘œ‚Ì•`‰æÀ•W

#pragma endregion

	// ‘I‘ğ’†
	SELECT select;

	// ‰æ‘œƒnƒ“ƒhƒ‹``````````````````

	// ƒ|ƒbƒvƒAƒbƒvƒtƒŒ[ƒ€
	int popupFrameImage;

	// ‘I‘ğˆ
	int selectImage[(int)SELECT::Max];

	// ‘I‘ğ’†‚ğ¦‚·ƒtƒŒ[ƒ€
	int selectFrameImage;

	// ‘I‘ğ’†‚ğ¦‚·–îˆó
	int selectArrowImage;

	// ‘I‘ğ’†‚Ìƒ{ƒ^ƒ“‚Ìã‚É•\¦‚·‚éŒˆ’èƒL[
	int enterKeyImage[2];

	// ``````````````````‰æ‘œƒnƒ“ƒhƒ‹

	// ‘I‘ğ’†‚ÌƒCƒ[ƒWŒQ‚ğ“®‚©‚·ƒJƒEƒ“ƒ^[
	float selectEasingCounter;
	float selectEasing;
};