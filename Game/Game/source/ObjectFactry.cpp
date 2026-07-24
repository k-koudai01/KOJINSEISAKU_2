#include "ObjectFactry.h"
#include "Cube.h"

std::unique_ptr<Player> ObjectFactry::CreatePlayer() const
{
	auto p = std::make_unique<Player>();	
	if(!p->Initialize())
	{
		return nullptr;
	}
	return p;
}

std::unique_ptr<Enemy> ObjectFactry::CreateEnemy() const
{
	auto e = std::make_unique<Enemy>();
	if(!e->Initialize())
	{
		return nullptr;
	}
	return e;
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

void ObjectFactry::SetUpEnemy(Enemy* enemy, Player* target) const
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