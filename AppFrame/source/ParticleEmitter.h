#pragma once
#include "appframe.h"
#include "Particle.h"

class ParticleEmitter
{
public:
	ParticleEmitter()  = default;
	~ParticleEmitter() = default;

	bool Initialize(int maxParticles, int textureHandle);
	bool Process(float deltaTime);
	bool Render();

	// パーティクルの発生
	bool Emit(const VECTOR& pos, const VECTOR& vel, float life, float scale = 1.0f);

	void Clear();

private:
	void UpdateParticle(Particle& p, float deltaTime);
	void RenderParticle(const Particle& p) const;

	Particle* FindDeadParticle();
private:
	std::vector<Particle> _pool;
	int _handle = -1;
};

