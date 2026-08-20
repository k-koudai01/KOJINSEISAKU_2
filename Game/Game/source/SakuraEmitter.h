#pragma once
#include "appframe.h"

class SakuraEmitter : public ParticleEmitter
{
public:
	SakuraEmitter()  = default;
	~SakuraEmitter() = default;

	bool Initialize();

	// 発生処理
	void SpawnWarmupParticles();						           // 初期発生
	bool EmitSakura(const VECTOR& pos);					           // 桜の発生
	void UpdateAutoEmit(const VECTOR& centerPos, float deltaTime); // 自動発生

	// 自動発生のON/OFF切り替え
	void SetSakuraAutoEmit(bool enable) { _isAutoEmit = enable; }
	bool IsSakuraAutoEmit() const		{ return _isAutoEmit;   }

	// パラメーター関数
	void SetEmitInterval(float interval)      { _emitInterval = interval; }
	void SetSpawnOffset(const VECTOR& offset) { _spawnOffset  = offset;	  }
	void SetSpawnRange(const VECTOR& range)   { _spawnRange   = range;    }
protected:
	void UpdateParticle(Particle& p, float deltaTime) override;
private:
	bool	_isAutoEmit  = true; // 発生フラグ
	float  _emitTimer    = 0.0f; 
	float  _emitInterval = 0.08f;					      // 発生間隔						  
	VECTOR _spawnOffset  = VGet(-530.0f, 230.0f, 0.0f);
	VECTOR _spawnRange   = VGet(400.0f, 250.0f, 200.0f);  // 発生範囲
};

