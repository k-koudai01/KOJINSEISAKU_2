#include "BulletManager.h"
#include "PlayerNormalBullet.h"
#include "PlayerChargeBullet.h"

bool BulletManager::Initialize()
{
	_bullets.clear();
	return true;
}

bool BulletManager::Terminate()
{
	for(auto& b : _bullets)
	{
		b->Terminate();
	}
	_bullets.clear();
	return true;
}

bool BulletManager::Process()
{
	for(auto& b : _bullets)
	{
		if(b->IsActive())
		{
			b->Process();
		}
	}

	_bullets.erase
	(
		// 寿命が尽きた弾を削除する
		std::remove_if(_bullets.begin(), _bullets.end(),
					   [](const std::unique_ptr<Bullet>& b)
					   {
						return !b->IsActive(); 
					   }),
		_bullets.end()
	);
	return true;
}

bool BulletManager::Render()
{
	for(auto& b : _bullets)
	{
		if(b->IsActive())
		{
			b->Render();
		}
	}
	return true;
}
