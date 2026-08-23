#include "SakuraEmitter.h"

namespace
{
	constexpr const char* SAKURA_TEXTURE_PATH   = "res/Effect/sakura.png";
	constexpr int		  MAX_SAKURA_PARTICLES  = 1000;
	constexpr int         WARMUP_PARTICLE_COUNT = 50;

	// 初期配置用パラメーター
	const VECTOR WARMUP_SPAWN_CENTER = VGet(0.0f, 200.0f, 0.0f);
	const VECTOR WARMUP_SPAWN_RANGE  = VGet(800.0f, 800.0f, 800.0f);

	// 風の基本パラメーター
	const VECTOR WIND_DIRECTION   = VGet(1.0f, -0.4f, 0.0f); // 風の方向
	constexpr float WIND_STRENGTH = 120.0f;				     // 風の強さ

	// 風の揺らぎ(Sin / Cos)パラメーター
	constexpr float WIND_SWAY_FREQ  = 4.0f;  // 横風の揺れの周波数
	constexpr float WIND_SWAY_AMP   = 20.0f; // 横風の揺れの振幅
	constexpr float FLOAT_UP_FREQ   = 3.0f;  // 上下のフワフワ感の周波数
	constexpr float FLOAT_UP_AMP    = 10.0f; // 上下のフワフワ感の振幅	

	// 粒子パラメーター
	constexpr float LIFE_MIN  = 10.0f;
	constexpr float LIFE_MAX  = 15.0f;
	constexpr float SCALE_MIN = 10.0f;
	constexpr float SCALE_MAX = 10.4f;

	// ランダムの範囲
	const VECTOR RANDOM_VEL_RANGE = VGet(0.0f, 50.0f, 20.0f);
}

bool SakuraEmitter::Initialize()
{
	if(!base::Initialize(SAKURA_TEXTURE_PATH, MAX_SAKURA_PARTICLES))
	{
		return false;
	}
	return true;
}

void SakuraEmitter::SpawnWarmupParticles()
{
	if(!_isEmit) return;

	// ランダムで50個の桜を初期発生させる
	for(int i = 0; i < WARMUP_PARTICLE_COUNT; ++i)
	{
		VECTOR spawnPos = MyMath::GetRandomPosInRange(WARMUP_SPAWN_CENTER, WARMUP_SPAWN_RANGE);
		EmitSakura(spawnPos);
	}
}

bool SakuraEmitter::EmitSakura(const VECTOR& pos)
{
	VECTOR vel = VGet
	(
		MyMath::GetRandomFloat(-RANDOM_VEL_RANGE.x, RANDOM_VEL_RANGE.x),
		MyMath::GetRandomFloat(-RANDOM_VEL_RANGE.y, RANDOM_VEL_RANGE.y),
		MyMath::GetRandomFloat(-RANDOM_VEL_RANGE.z, RANDOM_VEL_RANGE.z)
	);

	float life  = MyMath::GetRandomFloat(LIFE_MIN, LIFE_MAX);
	float scale = MyMath::GetRandomFloat(SCALE_MIN, SCALE_MAX);

	return Emit(pos, vel, life, scale);
}

void SakuraEmitter::UpdateParticle(Particle& p, float deltaTime)
{
	base::UpdateParticle(p, deltaTime);

	if(!p.active) return;

	// ベースとなる風の移動(方向 * 強さ * 時間)
	p.position.x += WIND_DIRECTION.x * WIND_STRENGTH * deltaTime;
	p.position.y += WIND_DIRECTION.y * WIND_STRENGTH * deltaTime;
	p.position.z += WIND_DIRECTION.z * WIND_STRENGTH * deltaTime;

	// 時間経過で風が強くなったり上下に浮き沈みする揺れ
	float windBreeze = sinf(p.life * WIND_SWAY_FREQ) * WIND_SWAY_AMP * deltaTime; // 横風の強弱
	float floatUp    = cosf(p.life * FLOAT_UP_FREQ)  * FLOAT_UP_AMP * deltaTime;   // 上下のフワフワ感

	p.position.x += windBreeze;
	p.position.y += floatUp;
}

void SakuraEmitter::UpdateAutoEmit(const VECTOR& centerPos, float deltaTime)
{
	if(!_isEmit) return;

	if(_isFirstEmit)
	{
		SpawnWarmupParticles();
		_isFirstEmit = false;
	}
	_emitTimer += deltaTime;

	if(_emitTimer >= _emitInterval)
	{
		_emitTimer = 0.0f;
		
		VECTOR basePos = VAdd(centerPos, _spawnOffset);
		VECTOR spawnPos = MyMath::GetRandomPosInRange(basePos, _spawnRange);

		EmitSakura(spawnPos);
	}
}