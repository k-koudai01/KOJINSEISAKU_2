#pragma once
#include "SpriteCharaBase.h"
#include "Camera.h"
#include "appframe.h"

class Bullet : public SpriteCharaBase
{
	typedef SpriteCharaBase base;
public:
	Bullet();
	~Bullet() = default;

	bool Initialize(const VECTOR& pos, const VECTOR& dir, float speed, float maxlifeTime);
	bool Terminate();
	bool Process();
	bool Render();

	void UpdateLifeTimer();

	// 外部から状態をチェックする関数
	bool IsActive() const { return _isActive; }
	void Destroy() { _isActive = false; }

protected:
	float _lifeTimer; 
	bool _isActive; 
};

