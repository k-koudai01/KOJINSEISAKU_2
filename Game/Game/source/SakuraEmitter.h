#pragma once
#include "appframe.h"

class SakuraEmitter : public ParticleEmitter
{
public:
	SakuraEmitter()  = default;
	~SakuraEmitter() = default;

	bool Initialize();

	// 単発発生
	bool EmitSakura(const VECTOR& pos);

	// 自動継続発生
	void UpdateAutoEmit(const VECTOR& centerPos, float deltaTime);

	// パラメーター関数
	void SetEmitInterval(float interval) { _emitInterval = interval; }
	void SetSpawnOffset(const VECTOR& offset) { _spawnOffset = offset; }
	void SetSpawnRange(const VECTOR& range) { _spawnRange = range; }
protected:
	void UpdateParticle(Particle& p, float deltaTime) override;
private:
	float  _emitTimer    = 0.0f; 
	float  _emitInterval = 0.08f;					      // 発生間隔
	int	   _emitCount    = 1;						      // 発生数

	VECTOR _spawnOffset  = VGet(-500.0f, 250.0f, 0.0f);
	VECTOR _spawnRange   = VGet(400.0f, 250.0f, 200.0f);  // 発生範囲
};

