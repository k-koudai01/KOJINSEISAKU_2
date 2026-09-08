#include "HitStopManager.h"

HitStopManager& HitStopManager::GetInstance()
{
	static HitStopManager instance;
	return instance;
}

void HitStopManager::Update(float rawDeltaTime)
{
	_rawDeltaTime = rawDeltaTime;

	if(_hitStopTimer > 0.0f)
	{
		_hitStopTimer -= rawDeltaTime;
		if(_hitStopTimer <= 0.0f)
		{
			_hitStopTimer = 0.0f;
			_hitStopScale = 1.0f;
		}
	}
}

void HitStopManager::RequestHitStop(float duration, float scale)
{
	// if(duration <= 0.0f) return; 
	_hitStopDuration = duration;
	_hitStopTimer	 = duration;
	_hitStopScale	 = scale;
}

float HitStopManager::GetDeltaTime() const
{
	return _rawDeltaTime * _hitStopScale;
}

bool HitStopManager::IsHitStopping() const
{
	return _hitStopTimer > 0.0f;
}