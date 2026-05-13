#include "ObjectFactry.h"

std::unique_ptr<Player> ObjectFactry::CreatePlayer() const
{
	auto p = std::make_unique<Player>();	

	if(!p->Initialize())
	{
		return nullptr;
	}

	return p;
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

void ObjectFactry::SetUpCamera(Camera* camera, Player* target) const
{
	if(camera == nullptr) return;

	camera->SetFollowTarget(target);

	if(target)
	{
		target->SetCamera(camera);
		camera->FollowUpdate();
	}
	
}