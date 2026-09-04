#include "ObjectFactry.h"
#include "Cube.h"
#include "EnemyBoss.h"
#include "MinionChaser.h"

ObjectFactry::ObjectFactry()
{
	RegisterEnemy("Boss",   []() { return std::make_unique<EnemyBoss>();    });
	RegisterEnemy("Chaser", []() { return std::make_unique<MinionChaser>(); });
}

std::unique_ptr<Player> ObjectFactry::CreatePlayer() const
{
	auto p = std::make_unique<Player>();	
	if(!p->Initialize())
	{
		return nullptr;
	}
	return p;
}

std::unique_ptr<EnemyBase> ObjectFactry::CreateEnemy(const std::string& typeName) const
{
	auto it = _enemyRegistry.find(typeName);
	if(it != _enemyRegistry.end())
	{
		auto enemy = it->second();
		if(enemy && enemy->Initialize())
		{
			return enemy;

		}
	}
	return nullptr;
}

std::unique_ptr<EnemyBoss> ObjectFactry::CreateEnemyBoss(Player* target, EnemyBoss::OnSpawnSpawnerCallback spawnerCallback) const
{
	auto baseEnemy = CreateEnemy("Boss");
	if(!baseEnemy) return nullptr;

	auto boss = std::unique_ptr<EnemyBoss>(dynamic_cast<EnemyBoss*>(baseEnemy.release()));
	if(boss)
	{
		boss->SetPlayer(target);
		SetUpEnemy(boss.get(), target);
		boss->SetOnSpawnSpawnerCallback(spawnerCallback);

		return boss;
	}
	return nullptr;
}

void ObjectFactry::RegisterEnemy(const std::string& typeName, EnemyCreator creator)
{
	_enemyRegistry[typeName] = creator;
}

std::unique_ptr<Camera> ObjectFactry::CreateCamera() const
{
	auto c = std::make_unique<Camera>();
	if(!c->Initialize())
	{
		return nullptr;
	}
	return c;
}

void ObjectFactry::SetUpCamera(Camera* camera, Player* target, bool isFixed) const 
{
	if(camera == nullptr) return;

	camera->SetFollowTarget(target);

	if(target)
	{
		target->SetCamera(camera);
		camera->FollowUpdate();
	}
	
	camera->SetFixedMode(isFixed);
}

void ObjectFactry::SetUpEnemy(EnemyBase* enemy, Player* target) const
{
	if(enemy == nullptr) return;

	enemy->SetPlayer(target);
}

std::unique_ptr<ObjectBase> ObjectFactry::CreateStageObject(const std::string& type) const
{
	if(type == "Cube")
	{
		auto cube = std::make_unique<Cube>();
		if(cube->Initialize())
		{
			return cube;
		}
	}

	return nullptr;
}