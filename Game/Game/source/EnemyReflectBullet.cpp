#include "EnemyReflectBullet.h"

namespace
{
	constexpr float MAX_HP = 2.0f; 
	constexpr float DAMAGE = 1.0f;
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
	_spriteScale = 120.0f;
	_isReflected = false;
	_damage      = DAMAGE;
	_hp			 = MAX_HP;

	return true;
}

bool EnemyReflectBullet::Process()
{
	if(!_isActive) return false;

	if(!base::Process()) return false;

	if(_hitStopTime > 0.0f)
	{
		return true; // 座標更新をスキップして停止状態を維持
	}

	_vPos = VAdd(_vPos, VScale(_vDir, _speed));

	// 寿命やアニメーションの更新
	UpdateLifeTimer();

	return base::Process();
}

bool EnemyReflectBullet::Render()
{
	if(!_isActive) return false;

	// 振動を加算した座標を作成
	VECTOR renderPos = VAdd(_vPos, _shakeOffset);

	// 実体座標を退避させ、描画用座標に一瞬だけ差し替える
	VECTOR backupPos = _vPos;
	_vPos = renderPos;

	base::Render();

	// 実体座標を元に戻す
	_vPos = backupPos;

	// デバッグ用の3D球体
	unsigned int color = _isReflected ? GetColor(0, 150, 255) : GetColor(255, 50, 50);
	DrawSphere3D(renderPos, _radius, 8, color, GetColor(255, 255, 255), TRUE);

	return true;
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