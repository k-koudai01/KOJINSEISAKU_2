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

	// ヒットストップと振動を開始する関数
	void OnHitImpact(float hitStopTime, float shakeStrength);

protected:
	// メイン関数
	void UpdateLifeTimer();
	bool UpdateHitImpact();

	// メイン変数
	float  _speed;
	float  _lifeTimer;
	bool   _isActive;

	float  _hitStopTime   = 0.0f;	
	float  _shakeStrength = 0.0f;					// 振動の強さ
	VECTOR _shakeOffset   = VGet(0.0f, 0.0f, 0.0f); // 振動位置のオフセット
};

