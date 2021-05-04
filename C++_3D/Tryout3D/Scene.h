#pragma once

#include "Camera.h"

class Scene
{
	public:

		Scene();
		virtual ~Scene();

		virtual void render() = 0;
		virtual void Update(float dt, Camera *cam) = 0;
};