#pragma once

#include "../source/container.h"

class SpriteAnimationManager
{
public:
	struct Instance
	{
		int id;
		int frameCount;
		float fps;
		bool loop;
		float time;
		int frame;
		bool playing;
	};

	static SpriteAnimationManager* GetInstance();

	int Play(int frameCount, float fps, bool loop = true);
	void Stop(int id);
	void Update(float dt);
	int GetFrame(int id) const;
	bool IsPlaying(int id) const;

private:
	int CreateInstance(int frameCount, float fps, bool loop);

	std::unordered_map<int, Instance> _instance;
	int _nextId = 1;
};

