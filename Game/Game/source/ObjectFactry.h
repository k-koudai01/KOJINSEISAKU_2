#pragma once

#include "appframe.h"
#include "Player.h"
#include "EnemyBase.h"
#include "Camera.h"
#include "ObjectBase.h"

class ObjectFactry
{
public:
	using EnemyCreator = std::function<std::unique_ptr<EnemyBase>()>;

	ObjectFactry();

	// オブジェクト生成
	std::unique_ptr<Player> CreatePlayer() const;
	std::unique_ptr<EnemyBase> CreateEnemy(const std::string& typeName = "Boss") const;
	std::unique_ptr<Camera> CreateCamera() const;
	// ステージオブジェクトの生成
	std::unique_ptr<ObjectBase> CreateStageObject(const std::string& type) const;

	// オブジェクトのセットアップ
	void SetUpCamera(Camera* camera, Player* target, bool isFixed = false) const;
	void SetUpEnemy(EnemyBase* enemy, Player* target) const;
	
	// @brief 敵の登録
	// @param typeName: 敵の種類名
	// @param creator:  敵の生成関数
	void RegisterEnemy(const std::string& typeName, EnemyCreator creator);
private:
	std::unordered_map<std::string, EnemyCreator> _enemyRegistry;
};

