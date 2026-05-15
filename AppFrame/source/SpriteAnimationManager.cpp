#include "SpriteAnimationManager.h"

SpriteAnimationManager* SpriteAnimationManager::GetInstance()
{
	static SpriteAnimationManager instance;
	return &instance;
}

int SpriteAnimationManager::Play(int frameCount, float fps, bool loop)
{
	return CreateInstance(frameCount, fps, loop);
}

void SpriteAnimationManager::Stop(int id)
{
	auto it = _instance.find(id);
	if(it != _instance.end())
	{
		_instance.erase(it);
	}
}

void SpriteAnimationManager::Update(float dt)
{
	for(auto& kv : _instance)
	{
		auto& inst = kv.second;
		if(!inst.playing) continue;

		inst.time += dt;
		float frameTime = 1.0f / inst.fps;

		while(inst.time >= frameTime)
		{
			inst.time -= frameTime;
			inst.frame++;

			// フレームが最後に達したらループか停止
			if (inst.frame >= inst.frameCount)
			{
				if(inst.loop) inst.frame = 0;
				{
					if(inst.loop) inst.frame = 0;
					else{ inst.frame = inst.frameCount - 1; inst.playing = false; }
				}
			}
		}
	}
}

int SpriteAnimationManager::GetFrame(int id) const
{
	auto it = _instance.find(id);
	if(it == _instance.end()) return 0;
	return  it->second.frame;
}

bool SpriteAnimationManager::IsPlaying(int id) const
{
	auto it = _instance.find(id);
	if(it == _instance.end()) return false;
	return it->second.playing;
}

int SpriteAnimationManager::CreateInstance(int frameCount, float fps, bool loop)
{
	Instance inst;
	inst.id = _nextId++;
	inst.frameCount = frameCount;
	inst.fps = fps;
	inst.loop = loop;
	inst.time = 0.0f;
	inst.frame = 0;
	inst.playing = true;

	_instance[inst.id] = inst;
	return inst.id;
}