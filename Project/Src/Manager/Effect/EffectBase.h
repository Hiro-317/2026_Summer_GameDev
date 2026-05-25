#pragma once

#include <EffekseerForDXLib.h>

#include "../../Object/Common/Transform.h"

enum class EFFECT_NAME {

	NON = -1,

	BOOM,

	MAX
};

struct EFFECT_INFO
{
	Transform trans{ Vector3() };
	int speed = -1;
};

class EffectBase {
public:

	EffectBase(void){}

	virtual ~EffectBase() = default;

	virtual void Load(void) = 0;

	virtual void Update(const Vector3* followPos)
	{
		if (playHandle != -1) {
			if (IsEffekseer3DEffectPlaying(playHandle) == -1) {
				end = true;
				return;
			}
			SetPosPlayingEffekseer3DEffect(playHandle, followPos->x, followPos->y, followPos->z);
			SetRotationPlayingEffekseer3DEffect(playHandle, info.trans.angle.x, info.trans.angle.y, info.trans.angle.z);
			SetScalePlayingEffekseer3DEffect(playHandle, info.trans.scale.x, info.trans.scale.y, info.trans.scale.z);
			UpdateEffekseer3D();
		}
	}
	virtual void Release(void) 
	{
		DeleteEffekseerEffect(info.trans.model);
	}

	bool IsEnd(void) { return end; }

protected:

	EFFECT_INFO info{};

	int playHandle = -1;

	bool end = false;
};