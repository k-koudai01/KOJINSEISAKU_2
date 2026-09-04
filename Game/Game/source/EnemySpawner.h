#pragma once
#include "appframe.h"
#include "Player.h"
#include "EnemyBase.h"

class EnemySpawner
{
public:
	using OnSpawnCallback = std::function<void(std::unique_ptr<EnemyBase>)>;

	EnemySpawner(const VECTOR& pos, const std::string& enemyType, Player* target, float spawnDelay);
	~EnemySpawner() = default;

	bool Update(float deltaTime, OnSpawnCallback onSpawn);
	bool Render();

	bool IsDead() const { return _isDead; }

protected:
	VECTOR _vPos;
	std::string _enemyType;
	Player* _player   = nullptr;
	float _timer	  = 0.0f;
	float _spawnDelay = 5.0f;
	bool _isDead      = false;
};

