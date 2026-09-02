#pragma once

#include "appframe.h"
#include "Player.h"
#include "EnemyBase.h"
#include "Camera.h"
#include "ObjectBase.h"

/**
 * @brief エネミーの種類
 */
enum class EnemyType
{
	Boss,   // ボス
	Minion  // 雑魚（将来の追加用）
};

class ObjectFactry
{
public:
	// オブジェクト生成
	std::unique_ptr<Player> CreatePlayer() const;
	std::unique_ptr<EnemyBase> CreateEnemy(EnemyType type = EnemyType::Boss) const;
	std::unique_ptr<Camera> CreateCamera() const;
	// ステージオブジェクトの生成
	std::unique_ptr<ObjectBase> CreateStageObject(const std::string& type) const;

	// オブジェクトのセットアップ
	void SetUpCamera(Camera* camera, Player* target, bool isFixed = false) const;
	void SetUpEnemy(EnemyBase* enemy, Player* target) const;
	
};

