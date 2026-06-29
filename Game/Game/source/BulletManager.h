#pragma once
#include "appframe.h"
#include "Bullet.h"

class BulletManager
{
public:
	static BulletManager* GetInstance()
	{
		static BulletManager instance;
		return &instance;
	}

	bool Initialize();
	bool Terminate();
	bool Process();
	bool Render();

	void Spawn(const VECTOR& pos, const VECTOR& dir, float speed, float maxLifeTime);

private:
	BulletManager()  = default;
	~BulletManager() = default;

	// 弾
	std::vector<std::unique_ptr<Bullet>> _bullets;
};

