#pragma once

#include "Camera.h"

class CurrentCamera
{
public:

	// æ“¾
	static Camera& Get(void) {
		if (camera == nullptr) { throw std::runtime_error("ƒJƒƒ‰‚ª‘¶İ‚µ‚È‚¢ó‘Ô‚ÅCurrentCamera‚Ö‚ÌQÆ‚ª‚ ‚è‚Ü‚µ‚½"); }
		return *camera;
	}
	// İ’è
	static void Set(Camera* currentCamera) { camera = currentCamera; }

private:

	// Œ»İ‚ÌƒJƒƒ‰
	static Camera* camera;

	CurrentCamera() = default;
	~CurrentCamera() = default;
};