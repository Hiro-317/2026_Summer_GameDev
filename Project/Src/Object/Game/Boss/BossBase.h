#pragma once

#include <array>
#include <cstddef>

#include "../../../Application/Application.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../Common/CharacterBase/CharacterBase.h"

#include "../../../Manager/Shader/ShaderTypeDefine.h"

class HitUI;
class HitSparkActor;

/// <summary>Šeƒ{ƒX‚É‹¤’Ê‚·‚é”íƒ_ƒ[ƒWAƒ^[ƒQƒbƒg‘I‘ğA’ÊM“¯Šú‚ğˆµ‚¤Šî’êƒNƒ‰ƒXB</summary>
class BossBase : public CharacterBase
{
public:

	/// <summary>Ú‘±l”‚É‘Î‰‚µ‚½”\—Í’l‚Æƒ{ƒXƒ‚ƒfƒ‹‚ğ“Ç‚İ‚ŞB</summary>
	/// <param name="parameterPath">ƒ{ƒX‚Ì”\—Í’lE‹““®ƒpƒ‰ƒ[ƒ^[‚ÌŠi”[æB</param>
	/// <param name="modelPath">Character/‚©‚ç‚Ì‘Š‘Îƒ‚ƒfƒ‹ƒpƒXB</param>
	BossBase(const std::string& parameterPath, std::string modelPath);

	~BossBase()override = default;

	/// <summary>ŒÅ—Lƒf[ƒ^‚ğ“Ç‚İ‚İA‹¤’ÊƒRƒ‰ƒCƒ_[EUIEƒqƒbƒg‰‰o‚ğ¶¬‚·‚éB</summary>
	void Load(void)override;

	/// <summary>ƒzƒXƒg‘¤‚Å”íƒ_ƒ[ƒW‚ğŠm’è‚µAƒqƒbƒg’Ê’m‚Æ€–Só‘Ô‚Ö‚Ì‘JˆÚ‚ğs‚¤B</summary>
	/// <param name="ownTag">Õ“Ë‚µ‚½©•ª‘¤‚ÌƒRƒ‰ƒCƒ_[ƒ^ƒOB</param>
	/// <param name="other">‘Šè‘¤‚ÌƒRƒ‰ƒCƒ_[BUŒ‚‚Í”\—Í’l‚ÆUŒ‚Ò‚ğQÆ‚·‚éB</param>
	/// <param name="result">ƒqƒbƒg‰‰o‚Æƒ_ƒ[ƒW•\¦‚Ég‚¤Õ“Ëî•ñB</param>
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	/// <summary>óM‚µ‚½ˆÊ’uAƒAƒjƒ[ƒVƒ‡ƒ“A”íƒ_ƒ[ƒWAƒ^[ƒQƒbƒgî•ñ‚ğ”½‰f‚·‚éB</summary>
	virtual void ReceptionUpdate(void)override;
	/// <summary>‘€ìŒ ‚ğ‚Â’[––‚©‚çƒ{ƒX‚ÌˆÊ’uE‰ñ“]EŠgk‚ğ‘—M‚·‚éB</summary>
	virtual void SendUpdate(void)override;

	/// <summary>€–SƒXƒe[ƒg‚Åİ’è‚³‚ê‚é€–Sƒtƒ‰ƒO‚ğQÆ‚·‚éB</summary>
	const bool& GetIsDeath(void)const { return isDeath; }

	/// <summary>‘ÎÛƒvƒŒƒCƒ„[‚ÌÀ•W‚ğØ—p‚·‚éBQÆæ‚Íƒ{ƒX‚Ì—˜—p’†A—LŒø‚Å‚ ‚é•K—v‚ª‚ ‚éB</summary>
	/// <param name="id">”z—ñ“Yš‚Æ‚µ‚Äg‚¤—LŒø‚È‘—MÒIDB</param>
	/// <param name="pos">’ÇÕ‘ÎÛ‚ÌÀ•WBŠ—LŒ ‚ÍˆÚ‚³‚È‚¢B</param>
	void SetPlayerPos(MSG_SENDER_ID id, const Vector3* pos) { playerPos[(int)id] = pos; }
	/// <summary>‘ÎÛƒvƒŒƒCƒ„[‚Ì€–Sƒtƒ‰ƒO‚ğØ—p‚·‚éBƒ^[ƒQƒbƒgXV‘O‚Éİ’è‚·‚éB</summary>
	/// <param name="id">”z—ñ“Yš‚Æ‚µ‚Äg‚¤—LŒø‚È‘—MÒIDB</param>
	/// <param name="isDeath">€–S‚Étrue‚É‚È‚éƒtƒ‰ƒOBŠ—LŒ ‚ÍˆÚ‚³‚È‚¢B</param>
	void SetPlayerIsDeath(MSG_SENDER_ID id, const bool* isDeath) { playerIsDeath[(int)id] = isDeath; }

private:

	// ƒƒCƒ“ˆ—``````````````
	void SubInit(void)override;
	void SubUpdate(void)override;
	void SubRelease(void)override;

	// ƒ{ƒX‚ªŠ—L‚·‚éÄ—˜—p—p‚Ìƒqƒbƒg‰‰oB“¯ƒqƒbƒg‚ğ•ÊX‚ÌˆÊ’u‚ÅÄ¶‚·‚éB
	std::array<HitSparkActor*, 16> hitEffects{};
	std::size_t nextHitEffect = 0;
	/// <summary>‹ó‚«‰‰o‚ğ—Dæ‚µA‹ó‚«‚ª‚È‚¯‚ê‚Î„‰ñˆÊ’u‚Ì‰‰o‚ğÄ—˜—p‚·‚éB</summary>
	/// <param name="point">ƒ[ƒ‹ƒhÀ•W‚ÌÕ“ËˆÊ’uB</param>
	/// <param name="critical">ƒNƒŠƒeƒBƒJƒ‹—p‚Ì‰‰o‚ğg‚¤‚©B</param>
	void PlayHitEffect(const Vector3& point, bool critical);
	// ``````````````ƒƒCƒ“ˆ—

protected:

	// ”h¶ƒNƒ‰ƒX‚Íƒ‚ƒfƒ‹ŒÅ—L‚ÌƒAƒjƒ[ƒVƒ‡ƒ“AUŒ‚—pActorAƒXƒe[ƒg‚ğ“o˜^‚·‚éB
	virtual void BossLoad(void) = 0;
	virtual void BossInit(void) {}
	virtual void BossUpdate(void) {}

	// ƒXƒe[ƒg‘JˆÚŠÖ”
	void ChangeState(int nextState)override;
	using CharacterBase::ChangeState;

	// ƒ‚ƒfƒ‹ƒAƒjƒ[ƒVƒ‡ƒ“ƒvƒŒƒCŠÖ”
	void AnimePlay(int type, signed char loop = -1)override;
	using CharacterBase::AnimePlay;

	virtual void PlayDamage(void){}

	virtual void DefaultCreateShader(void)override;

	virtual SHADER_VERTEX_TYPE GetVertexType(void) const { return SHADER_VERTEX_TYPE::Frame8; }

#pragma region ’è”’è‹`
	// ƒ‚ƒfƒ‹``````````````````````````````

	// ƒXƒP[ƒ‹
	const Vector3 MODEL_SCALE = GetParameterToVector3("Model", "Scale");

	// ƒTƒCƒY
	const Vector3 MODEL_SIZE = GetParameterToVector3("Model", "Size") * MODEL_SCALE;

	// ’†S“_‚ÌƒYƒŒ
	const Vector3 MODEL_CENTER_DIFF = GetParameterToVector3("Model", "CenterDiff") * MODEL_SCALE;

	// Šp“x‚ÌƒYƒŒ
	const Quaternion MODEL_LOCAL_ROT = Quaternion::FromEulerXZY(GetParameterToVector3("Model", "LocalRot") * (DX_PI_F / 180.0f));

	// `````````````````````````````````


	// “–‚½‚è”»’èî•ñ`````````````````````````````````````````

	// ‰Ÿ‚µo‚µ‚ğs‚¤Û‚Ìd‚³
	const unsigned char COLLISION_PUSH_WEIGHT = (unsigned char)GetParameterToInt("Collider", "CollisionPushWeight");

	// ````````````````````````````````````````````````


	// •Ï”‰Šú‰»Œn``````````````````````````

	// ‰ŠúÀ•W
	const Vector3 INIT_POS = GetParameterToVector3("Init", "Pos");
	const Vector3 INIT_ROT = GetParameterToVector3("Init", "Rot") * DX_PI_F / 180.0f;

	// ``````````````````````````•Ï”‰Šú‰»Œn

	// ˆÚ“®ó‘Ô`````````````````````````````

	// ˆÚ“®—Ê
	const float MOVE_SPEED = GetParameter("Move", "Speed");

	// `````````````````````````````ˆÚ“®ó‘Ô

	// `````````````````````````````ˆÚ“®ó‘Ô

	// ó‘Ô‚Ìí—Ş•m€
	enum class STATE
	{
		NONE = -1,

		IDLE,			// ‘Ò‹@ó‘Ô
		MOVE,			// ˆÚ“®ó‘Ô

		ATTACK_A,
		ATTACK_B,
		ATTACK_C,
		ATTACK_D,
		ATTACK_E,
		ATTACK_F,
		ATTACK_G,

		DEATH,

		MAX
	};

	// ‰æ‘œ‚Ì–¼‘O
	static const inline std::string HP_FRAME_IMAGE_NAME = "BossHpFrame.png";
	static const inline std::string HP_IMAGE_NAME = "BossHp.png";
	static const inline std::string HP_LOST_IMAGE_NAME = "BossHpLost.png";

	// HPƒo[‚Ì‰æ‘œƒTƒCƒY
	static const inline Vector2I HP_IMAGE_SIZE = Vector2I(629, 50);
	static const inline Vector2I HP_GAUGE_OFFSET = Vector2I(18, 0);
	static const inline Vector2I HP_UI_POS = Vector2I(App::SCREEN_SIZE_X_HALF - 314, (int)((float)HP_IMAGE_SIZE.y / 1.5f));

#pragma endregion ’è”’è‹`

	// ƒvƒŒƒCƒ„[‚ÌÀ•W‚ÌQÆ(“Ç‚İæ‚èê—p)
	const Vector3* playerPos[(int)MSG_SENDER_ID::Max];

	// ƒvƒŒƒCƒ„[‚Ì¶‘¶”»’è‚ÌQÆ(“Ç‚İæ‚èê—p)
	const bool* playerIsDeath[(int)MSG_SENDER_ID::Max];

	// ƒvƒŒƒCƒ„[‚Ì—^ƒ_ƒƒJƒEƒ“ƒg
	int damageDealtByPlayer[(int)MSG_SENDER_ID::Max]{};

	// Œ»İ‚Ìƒ^[ƒQƒbƒgƒvƒŒƒCƒ„[”Ô†
	int targetPlayerIndex;


	// ƒRƒ‰ƒCƒ_[‚Ì“–‚½‚è”»’è•`‰æ‚Ìƒpƒ‰ƒ[ƒ^[
	ParameterLoad* attackRangeParameters;

	// ƒ_ƒ[ƒWUIŠÇ—ƒNƒ‰ƒX
	HitUI* hitUiRef;

	// €–S”»’è
	bool isDeath = false;
};
