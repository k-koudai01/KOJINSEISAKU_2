#pragma once
#include "appframe.h"
class LeavesEmitter : public ParticleEmitter
{
public:
	LeavesEmitter() = default;
	~LeavesEmitter() override = default;

	bool Initialize();

	// 発生処理
	bool EmitLeaves(const VECTOR& pos);
	void UpdateAutoEmit(const VECTOR& centerPos, float deltaTime);

	// 自動発生のON/OFF切り替え
	void SetLeavesAutoEmit(bool enable) { _isAutoEmit = enable; }
	bool IsLeavesAutoEmit() const { return _isAutoEmit; }

	// パラメーター変更用セッター
	void SetEmitInterval(float interval) { _emitInterval = interval; }
	void SetSpawnOffset(const VECTOR& offset) { _spawnOffset = offset; }
	void SetSpawnRange(const VECTOR& range) { _spawnRange = range; }

protected:
	void UpdateParticle(Particle& p, float deltaTime) override;

private:
	void SpawnWarmupParticles();

private:
	bool   _isAutoEmit   = true;                     // 自動発生フラグ
	float  _emitTimer    = 0.0f;
	float  _emitInterval = 0.12f;                    // 発生間隔
	VECTOR _spawnOffset  = VGet(-500.0f, 250.0f, 0.0f);
	VECTOR _spawnRange   = VGet(400.0f, 250.0f, 200.0f); // 発生範囲

};

