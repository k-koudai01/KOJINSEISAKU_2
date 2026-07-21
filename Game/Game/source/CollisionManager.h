#pragma once
#include "appframe.h"
#include "Player.h"
#include "Enemy.h"
#include "Cube.h"
#include "Camera.h"
#include "CameraManager.h"

class CollisionManager
{
public:
	// 更新処理
	void CheckPlayerEnemy(Player* player, Enemy* enemy);
	void CheckPlayerAttack(Player* player, Enemy* enemy);
	void CheckCharacterCube(CharaBase* character, Cube* cube);
	// 描画処理
	void DebugRenderCapsule(const Player* player, const Enemy* enemy) const;
	
protected:
	// カメラ
	Camera* _cam;
};

