#include "Enemy.h"
#include "mymath.h"
#include "Player.h"
#include "DxLib.h"
#include <cstdio>

bool Enemy::Initialize()
{
	if(!base::Initialize()) { return false; }

	SetSpriteSheet(STATUS::IDLE, "res/Enemy/Enemy_Idle.png", 4, 4);
	
	SetSpriteAnimTable(
	{
		{ STATUS::IDLE, {  4,  5.0f, true  } },
	});

	_spriteScale = 80.0f;

	_status = STATUS::IDLE;

	_vPos = VGet(0.0f, 0.0f, 0.0f);
	_vDir = VGet(0.0f, 0.0f, -1.0f);

	_fColSubY = 17.0f;
	_fCollisionR = 11.0f;
	_fCollisionWeight = 20.0f;

	_mvSpeed = 0.0f;
	_hp = 5.0f;

	return true;
}

bool Enemy::Terminate()
{
	base::Terminate();
	return true;
}

bool Enemy::Process()
{
	base::Process();

	STATUS oldStatus = _status;
	_status = STATUS::IDLE;

	UpdateSpriteAnimation(oldStatus);

	return true;
}

bool Enemy::Render()
{
	return base::Render();
}