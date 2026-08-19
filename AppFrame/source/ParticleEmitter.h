#pragma once
#include "appframe.h"
#include "Particle.h"

class ParticleEmitter
{
public:
	ParticleEmitter()  = default;
	virtual ~ParticleEmitter() = default;

	virtual	bool Initialize(const char* texturePath, int maxParticles);
	virtual bool Process(float deltaTime);
	virtual bool Render();

	// パーティクルの発生
	bool Emit(const VECTOR& pos, const VECTOR& vel, float life, float scale = 1.0f);

	void Clear();

protected:
	virtual void UpdateParticle(Particle& p, float deltaTime);
	void RenderParticle(const Particle& p) const;

	Particle* FindDeadParticle();
protected:
	std::vector<Particle> _pool;
	int _handle = -1;
};

