#pragma once
#include "appframe.h"
#include "Particle.h"

class ParticleEmitter
{
public:
	ParticleEmitter()  = default;
	~ParticleEmitter() = default;

	bool Initialize(int maxParticles, int textureHandle);
	bool Process();
	bool Render();

	// パーティクルの発生
	bool Emit(const VECTOR& position);
};

