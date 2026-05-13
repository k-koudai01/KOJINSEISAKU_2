#include "ObjectManager.h"

void ObjectManager::Add(std::unique_ptr<ObjectBase> obj)
{
	if(!obj) { return; };

	_objects.emplace_back(std::move(obj));
}

void ObjectManager::ProcessAll()
{
	for(auto& obj : _objects)
	{
		if(obj) { obj->Process(); }
	}
}

void ObjectManager::RenderAll()
{
	for(auto& obj : _objects)
	{
		if(obj) { obj->Render(); }
	}
}

void ObjectManager::TerminateAll()
{
	for(auto& obj : _objects)
	{
		if(obj) { obj->Terminate(); }
	}
	_objects.clear();
}

void ObjectManager::Clear()
{
	_objects.clear();
}