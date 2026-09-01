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

	virtual bool Initialize(const VECTOR& pos, const VECTOR& dir, float speed, float maxLifeTime);
	virtual bool Process() override;
    virtual bool Render() override;

    // ゲッター / セッター
    bool IsActive() const { return _isActive; }
    void Destroy() { _isActive = false; }

protected:
	// メイン関数
	void UpdateLifeTimer();

	// メイン変数
	VECTOR _vDir;
	float  _speed;
	float  _lifeTimer;
	bool   _isActive;

};

