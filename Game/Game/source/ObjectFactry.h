#pragma once

#include "appframe.h"
#include "Player.h"
#include "Camera.h"

class ObjectFactry
{
public:
	std::unique_ptr<Player> CreatePlayer() const;
	std::unique_ptr<Camera> CreateCamera() const;
	void SetUpCamera(Camera* camera, Player* target) const;
};

