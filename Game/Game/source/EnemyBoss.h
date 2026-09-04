#pragma once
#include "EnemyBase.h"
#include "EnemySpawner.h"

class EnemyBoss : public EnemyBase
{
	typedef EnemyBase base;
public:
	
	// @brief スポーン生成をModeGameに伝えるためのコールバック
	using OnSpawnSpawnerCallback = std::function<void(std::unique_ptr<EnemySpawner>)>;

	EnemyBoss() = default;
	virtual ~EnemyBoss() = default;

	virtual bool Initialize() override;
	virtual bool Process() override;
	virtual bool Render() override;

	// @brief Minion召喚用のコールバックを設定する
	void SetOnSpawnSpawnerCallback(OnSpawnSpawnerCallback cb) { _onSpawnSpawnerCallback = cb; }

	// @brief Minion召喚処理
	void SpawnMinion();

private:
	OnSpawnSpawnerCallback _onSpawnSpawnerCallback = nullptr;
};

