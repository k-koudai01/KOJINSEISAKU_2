#pragma once
#include "appframe.h"
#include "Bullet.h"


class BulletManager
{
public:
	static BulletManager* GetInstance()
	{
		static BulletManager instance;
		return &instance;
	}

	bool Initialize();
	bool Terminate();
	bool Process();
	bool Render();

	template <typename T>
	void Spawn(const VECTOR& pos, const VECTOR& dir)
	{
		static_assert(std::is_base_of<Bullet, T>::value, "Bullet の派生クラスを指定してください");

		auto bullet = std::make_unique<T>();
		if(bullet->Initialize(pos, dir))
		{
			_bullets.push_back(std::move(bullet));
		}
	}

	const std::vector<std::unique_ptr<Bullet>>& GetBullets() const { return _bullets; }
private:
	// 弾
	std::vector<std::unique_ptr<Bullet>> _bullets;
};

