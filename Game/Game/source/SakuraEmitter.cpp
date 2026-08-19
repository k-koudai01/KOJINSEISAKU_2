#include "SakuraEmitter.h"

namespace
{
	constexpr const char* SAKURA_TEXTURE_PATH  = "res/Effect/sakura.png";
	constexpr int		  MAX_SAKURA_PARTICLES = 1000;
}

bool SakuraEmitter::Initialize()
{
	if(!ParticleEmitter::Initialize(SAKURA_TEXTURE_PATH, MAX_SAKURA_PARTICLES))
	{
		return false;
	}

	// ランダムで50個の桜を初期発生させる
	VECTOR initCenter = VGet(0.0f, 0.0f, 0.0f); 
	for(int i = 0; i < 50; ++i)
	{
		VECTOR spawnPos = VGet(
			(float)(GetRand(800) - 400),
			(float)(GetRand(800) - 200), 
			(float)(GetRand(800) - 400)
		);
		EmitSakura(spawnPos);
	}

	return true;
}

bool SakuraEmitter::EmitSakura(const VECTOR& pos)
{
	float windPower = 30.0f + (float)GetRand(90);
	float fallSpeed = -10.0f - (float)GetRand(35);

	VECTOR vel = VGet(
		windPower,
		fallSpeed,
		(float)(GetRand(100) - 50) * 0.1f
	);

	float life  = 10.0f + (float)GetRand(5); 
	float scale = 20.0f + (float)GetRand(4) * 0.1f;

	return Emit(pos, vel, life, scale);
}

void SakuraEmitter::UpdateParticle(Particle& p, float deltaTime)
{
	ParticleEmitter::UpdateParticle(p, deltaTime);

	if(!p.active) return;

	// 時間経過で風が強くなったり上下に浮き沈みする揺れ
	float windBreeze = sinf(p.life * 4.0f) * 20.0f * deltaTime; // 横風の強弱
	float floatUp    = cosf(p.life * 3.0f) * 10.0f * deltaTime; // 上下のフワフワ感

	p.position.x += windBreeze;
	p.position.y += floatUp;
}

void SakuraEmitter::UpdateAutoEmit(const VECTOR& centerPos, float deltaTime)
{
	_emitTimer += deltaTime;

	if(_emitTimer >= _emitInterval)
	{
		_emitTimer = 0.0f;
		
		for(int i = 0; i < _emitCount; ++i)
		{
			VECTOR basePos = VAdd(centerPos, _spawnOffset);

			int rangeX = static_cast<int>(_spawnRange.x);
			int rangeY = static_cast<int>(_spawnRange.y);
			int rangeZ = static_cast<int>(_spawnRange.z);

			VECTOR spawnPos = VGet(
				basePos.x + (float)(GetRand(rangeX) - rangeX / 2),
				basePos.y + (float)(GetRand(rangeY) - rangeY / 2),
				basePos.z + (float)(GetRand(rangeZ) - rangeZ / 2)
			);

			EmitSakura(spawnPos);
		}
	}
}