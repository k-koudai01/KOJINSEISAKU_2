#include "EnemySpawner.h"
#include "ObjectFactry.h"

EnemySpawner::EnemySpawner(const VECTOR& pos, const std::string& enemyType, Player* target, float spawnDelay)
	: _vPos(pos)
	, _enemyType(enemyType)
	, _player(target)
	, _spawnDelay(spawnDelay)
	, _timer(0.0f)
	, _isDead(false)
{
}

bool EnemySpawner::Update(float deltaTime, OnSpawnCallback onSpawn)
{
	if(_isDead) return false;

	_timer += deltaTime;
	if(_timer >= _spawnDelay)
	{
		ObjectFactry objFtr;
		auto newEnemy = objFtr.CreateEnemy(_enemyType);

		if(newEnemy)
		{
			newEnemy->SetPos(_vPos);
			newEnemy->SetPlayer(_player);

			if(onSpawn)
			{
				onSpawn(std::move(newEnemy));
			}
			_isDead = true;
			return true;
		}
	}

	return false;
}

bool EnemySpawner::Render()
{
	if(_isDead) { return false; }

	// 動作確認用：出現予定地に魔法陣をうっすら描画
	// タイマーが進むにつれて円が大きくなる
	float progress = _timer / _spawnDelay;
	DrawSphere3D(_vPos, 15.0f * progress, 8, GetColor(255, 50, 50), GetColor(255, 0, 0), FALSE);	
	
	return true;
}

