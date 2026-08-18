#include "ParticleEmitter.h"

bool ParticleEmitter::Initialize(int maxParticles, int textureHandle)
{
	if(_handle < 0) return false;

	_handle = textureHandle;

	// メモリを事前に確保
	_pool.resize(maxParticles);
	for(auto& p : _pool)
	{
		p.active = false; 
	}

	return true;
}

bool ParticleEmitter::Process(float deltaTime)
{
	for(auto& p : _pool)
	{
		if(!p.active) continue;

		UpdateParticle(p, deltaTime);
	}

	return true;
}

bool ParticleEmitter::Render()
{
	if(_handle < 0) return false;

	for(const auto& p : _pool)
	{
		if(!p.active) continue;

		RenderParticle(p);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	return true;
}

void ParticleEmitter::Clear()
{
	for(auto& p : _pool)
	{
		p.active = false;
	}
}

void ParticleEmitter::UpdateParticle(Particle& p, float deltaTime)
{
	// 寿命の更新
	p.life -= deltaTime;
	if(p.life <= 0.0f)
	{
		p.active = false; // 非アクティブ化
		return;
	}

	p.position = VAdd(p.position, VScale(p.velocity, deltaTime));
	p.rotation += p.rotSpeed * deltaTime;
}

void ParticleEmitter::RenderParticle(const Particle& p) const
{
	// 寿命に応じてアルファ値を計算
	float alphaRate = p.life / p.maxLife;
	int alpha = static_cast<int>(255.0f * alphaRate);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	// ビルボード描画
	DrawBillboard3D(p.position, 0.5f, 0.5f, p.scale.x, p.rotation, _handle, TRUE);
}

Particle* ParticleEmitter::FindDeadParticle()
{
	for(auto& p : _pool)
	{
		if(!p.active) return &p;
	}
	return nullptr; // 上限を達した場合
}

bool ParticleEmitter::Emit(const VECTOR& pos, const VECTOR& vel, float life, float scale)
{
	Particle* p = FindDeadParticle();
	if(!p) return false; 

	p->position = pos;
	p->velocity = vel;
	p->life = life;
	p->maxLife = life;
	p->scale = VGet(scale, scale, scale);
	p->rotation = 0.0f;
	p->rotSpeed = (GetRand(100) - 50) * 0.05f;
	p->active = true;

	return true;
}
