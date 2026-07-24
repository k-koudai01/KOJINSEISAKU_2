#pragma once

#include "appframe.h"
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "ObjectBase.h"

class ObjectFactry
{
public:
	// オブジェクト生成
	std::unique_ptr<Player> CreatePlayer() const;
	std::unique_ptr<Enemy> CreateEnemy() const;
	std::unique_ptr<Camera> CreateCamera() const;
	// ステージオブジェクトの生成
	std::unique_ptr<ObjectBase> CreateStageObject(const std::string& type) const;

	// オブジェクトのセットアップ
	void SetUpCamera(Camera* camera, Player* target, bool isFixed = false) const;
	void SetUpEnemy(Enemy* enemy, Player* target) const; 
	
};

