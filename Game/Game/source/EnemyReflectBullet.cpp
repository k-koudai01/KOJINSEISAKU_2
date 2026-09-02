#include "EnemyReflectBullet.h"

namespace
{
	constexpr float MAX_HP = 2.0f; 
}

bool EnemyReflectBullet::Initialize(const VECTOR& pos, const VECTOR& dir)
{
	if(!base::Initialize(pos, dir, 10.0f, 8.0f))
	{
		return false;
	}

	// スプライト・コリジョン初期化
	SetSpriteSheet(STATUS::IDLE, "res/Effect/Attack_0.png", 3, 4);
	SetSpriteAnimTable({ { STATUS::IDLE, { 3, 30.0f, true } } });

	_status      = STATUS::IDLE;
	_fCollisionR = 25.0f;
	_radius      = 25.0f; 
	_speed		 = 2.0f;
	_spriteScale = 100.0f;
	_isReflected = false;
	_hp			 = MAX_HP;

	return true;
}

bool EnemyReflectBullet::Process()
{
	if(!_isActive) return false;

	_vPos = VAdd(_vPos, VScale(_vDir, _speed));

	// 寿命やアニメーションの更新
	UpdateLifeTimer();

	return base::Process();
}

bool EnemyReflectBullet::Render()
{
	if(!_isActive) return false;

	unsigned int color = _isReflected ? GetColor(0, 150, 255) : GetColor(255, 50, 50);
	DrawSphere3D(_vPos, _radius, 8, color, GetColor(255, 255, 255), TRUE);

	return base::Render();
}

bool EnemyReflectBullet::Damage(float damage)
{
	if(_isReflected || !_isAlive) return false;

	_hp -= damage;

	// 0以下になったら跳ね返し処理を実行！
	if(_hp <= 0.0f)
	{
		_hp = 0.0f;
	}

	return true;
}

void EnemyReflectBullet::Reflect(const VECTOR& newDir)
{
	// すでに跳ね返し済みなら何もしない
	if(_isReflected) return;

	_isReflected = true; 

	// 新しい方向ベクトルを正規化してセットする
	float dirLength = VSize(newDir);
	_vDir = (dirLength > 0.0f) ? VNorm(newDir) : VGet(0.0f, 0.0f, 1.0f);

	// 少しだけ弾速アップ
	_speed *= 1.2f;
}