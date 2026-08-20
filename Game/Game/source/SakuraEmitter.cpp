#include "SakuraEmitter.h"

namespace
{
	constexpr const char* SAKURA_TEXTURE_PATH   = "res/Effect/sakura.png";
	constexpr int		  MAX_SAKURA_PARTICLES  = 1000;
	constexpr int         WARMUP_PARTICLE_COUNT = 50;

	// 初期配置用パラメーター
	const VECTOR WARMUP_SPAWN_CENTER = VGet(0.0f, 200.0f, 0.0f);
	const VECTOR WARMUP_SPAWN_RANGE  = VGet(800.0f, 800.0f, 800.0f);

	// 速度パラメーター
	constexpr float VEL_X_MIN = 30.0f;
	constexpr float VEL_X_MAX = 120.0f;
	constexpr float VEL_Y_MIN = -45.0f;
	constexpr float VEL_Y_MAX = -10.0f;
	constexpr float VEL_Z_MIN = -5.0f;
	constexpr float VEL_Z_MAX = 5.0f;

	// 粒子パラメーター
	constexpr float LIFE_MIN  = 10.0f;
	constexpr float LIFE_MAX  = 15.0f;
	constexpr float SCALE_MIN = 10.0f;
	constexpr float SCALE_MAX = 10.4f;

	// 風の揺らぎ(Sin / Cos)パラメーター
	constexpr float WIND_SWAY_FREQ  = 4.0f;  // 横風の揺れの周波数
	constexpr float WIND_SWAY_AMP   = 20.0f; // 横風の揺れの振幅
	constexpr float FLOAT_UP_FREQ   = 3.0f;  // 上下のフワフワ感の周波数
	constexpr float FLOAT_UP_AMP    = 10.0f; // 上下のフワフワ感の振幅	
}

bool SakuraEmitter::Initialize()
{
	if(!ParticleEmitter::Initialize(SAKURA_TEXTURE_PATH, MAX_SAKURA_PARTICLES))
	{
		return false;
	}

	// ランダムで50個の桜を初期発生させる
	for(int i = 0; i < WARMUP_PARTICLE_COUNT; ++i)
	{
		VECTOR spawnPos = MyMath::GetRandomPosInRange(WARMUP_SPAWN_CENTER, WARMUP_SPAWN_RANGE);
		EmitSakura(spawnPos);
	}

	return true;
}

bool SakuraEmitter::EmitSakura(const VECTOR& pos)
{
	VECTOR vel = VGet
	(
		MyMath::GetRandomFloat(VEL_X_MIN, VEL_X_MAX),
		MyMath::GetRandomFloat(VEL_Y_MIN, VEL_Y_MAX),
		MyMath::GetRandomFloat(VEL_Z_MIN, VEL_Z_MAX)
	);

	float life  = MyMath::GetRandomFloat(LIFE_MIN, LIFE_MAX);
	float scale = MyMath::GetRandomFloat(SCALE_MIN, SCALE_MAX);

	return Emit(pos, vel, life, scale);
}

void SakuraEmitter::UpdateParticle(Particle& p, float deltaTime)
{
	ParticleEmitter::UpdateParticle(p, deltaTime);

	if(!p.active) return;

	// 時間経過で風が強くなったり上下に浮き沈みする揺れ
	float windBreeze = sinf(p.life * WIND_SWAY_FREQ) * WIND_SWAY_AMP * deltaTime; // 横風の強弱
	float floatUp    = cosf(p.life * FLOAT_UP_FREQ) * FLOAT_UP_AMP * deltaTime;   // 上下のフワフワ感

	p.position.x += windBreeze;
	p.position.y += floatUp;
}

void SakuraEmitter::UpdateAutoEmit(const VECTOR& centerPos, float deltaTime)
{
	_emitTimer += deltaTime;

	if(_emitTimer >= _emitInterval)
	{
		_emitTimer = 0.0f;
		
		VECTOR basePos = VAdd(centerPos, _spawnOffset);
		VECTOR spawnPos = MyMath::GetRandomPosInRange(basePos, _spawnRange);

		EmitSakura(spawnPos);
	}
}