#include "LeavesEmitter.h"

namespace
{
	constexpr const char* LEAF_TEXTURE_PATHS[] =
	{
		"res/Effect/leaf_01.png",
		"res/Effect/leaf_02.png"
	};

	constexpr int LEAF_TEXTURE_COUNT = static_cast<int>(std::size(LEAF_TEXTURE_PATHS));

	// 基本パラメータ
	constexpr int MAX_LEAF_PARTICLES    = 500;
	constexpr int WARMUP_PARTICLE_COUNT = 30;

	// 初期配置用パラメータ
	const VECTOR WARMUP_SPAWN_CENTER = VGet(0.0f, 200.0f, 0.0f);
	const VECTOR WARMUP_SPAWN_RANGE  = VGet(800.0f, 800.0f, 800.0f);

	// 風の基本設定
	const VECTOR WIND_DIRECTION   = VGet(1.2f, -0.5f, 0.0f);
	constexpr float WIND_STRENGTH = 60.0f;

	// 葉っぱ特有のゆらぎ設定
	constexpr float WIND_SWAY_FREQ = 2.5f;  // 周期を少し遅く
	constexpr float WIND_SWAY_AMP  = 35.0f; // 左右の揺れ幅を大きめに
	constexpr float FLOAT_UP_FREQ  = 2.0f;
	constexpr float FLOAT_UP_AMP   = 15.0f;

	// 初期散らばり・粒子パラメータ
	const VECTOR RANDOM_VEL_RANGE = VGet(15.0f, 10.0f, 3.0f);
	constexpr float LIFE_MIN      = 8.0f;
	constexpr float LIFE_MAX      = 12.0f;
	constexpr float SCALE_MIN     = 12.0f; 
	constexpr float SCALE_MAX	  = 15.0f;
}

bool LeavesEmitter::Initialize()
{
	// テーブル内をループ読み込み
	_leafHandles.clear();
	for(int i = 0; i < LEAF_TEXTURE_COUNT; ++i)
	{ 
		int handle = LoadGraph(LEAF_TEXTURE_PATHS[i]);
		if(handle == -1) { return false; }
		_leafHandles.push_back(handle);
	}

	if(!base::Initialize(LEAF_TEXTURE_PATHS[0], MAX_LEAF_PARTICLES))
	{
		return false;
	}

	return true;
}

void LeavesEmitter::SpawnWarmupParticles()
{
	for(int i = 0; i < WARMUP_PARTICLE_COUNT; ++i)
	{
		VECTOR spawnPos = MyMath::GetRandomPosInRange(WARMUP_SPAWN_CENTER, WARMUP_SPAWN_RANGE);
		EmitLeaves(spawnPos);
	}
}

bool LeavesEmitter::EmitLeaves(const VECTOR& pos)
{
	VECTOR vel = VGet(
		MyMath::GetRandomFloat(-RANDOM_VEL_RANGE.x, RANDOM_VEL_RANGE.x),
		MyMath::GetRandomFloat(-RANDOM_VEL_RANGE.y, RANDOM_VEL_RANGE.y),
		MyMath::GetRandomFloat(-RANDOM_VEL_RANGE.z, RANDOM_VEL_RANGE.z)
	);

	float life  = MyMath::GetRandomFloat(LIFE_MIN, LIFE_MAX);
	float scale = MyMath::GetRandomFloat(SCALE_MIN, SCALE_MAX);

	// ランダムに画像を選択
	int index = static_cast<int>(MyMath::GetRandomFloat(0.0f, static_cast<float>(LEAF_TEXTURE_COUNT)));
	if(index >= static_cast<int>(_leafHandles.size())) index = static_cast<int>(_leafHandles.size()) - 1;
	int selectedHandle = _leafHandles[index];

	return Emit(pos, vel, life, scale, selectedHandle);
}

void LeavesEmitter::UpdateParticle(Particle& p, float deltaTime)
{
	base::UpdateParticle(p, deltaTime);

	if(!p.active) return;

	// 風の移動
	p.position.x += WIND_DIRECTION.x * WIND_STRENGTH * deltaTime;
	p.position.y += WIND_DIRECTION.y * WIND_STRENGTH * deltaTime;
	p.position.z += WIND_DIRECTION.z * WIND_STRENGTH * deltaTime;

	// 葉っぱ特有の揺れを追加
	float windBreeze = sinf(p.life * WIND_SWAY_FREQ) * WIND_SWAY_AMP * deltaTime;
	float floatUp    = cosf(p.life * FLOAT_UP_FREQ ) * FLOAT_UP_AMP  * deltaTime;

	p.position.x += windBreeze;
	p.position.y += floatUp;
}

void LeavesEmitter::UpdateAutoEmit(const VECTOR& centerPos, float deltaTime)
{
	if(!_isEmit) return;

	// 初回発生
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

		EmitLeaves(spawnPos);
	}
}