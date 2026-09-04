#include "MinionRunner.h"

bool MinionRunner::Initialize()
{
	if(!base::Initialize()) { return false; }

	SetSpriteSheet(STATUS::IDLE, "res/Enemy/Enemy_Idle.png", 4, 4);
	SetSpriteSheet(STATUS::WALK, "res/Enemy/Enemy_Run.png" , 8, 4);

	SetSpriteAnimTable
	({
		{ STATUS::IDLE, { 4, 10.0f, true } },
		{ STATUS::WALK, { 8, 10.0f, true } },
	});

	_spriteScale = 100.0f;
	_hp = 1.0f;
	_mvSpeed = 4.0f;

	_fColSubY		  = 10.0f;
	_fCollisionR	  = 20.0f;
	_fCollisionWeight = 5.0f;

	_status = STATUS::WALK;

	return true;
}

bool MinionRunner::Process()
{
	base::Process();
	UpdateMove();
	return true;
}

bool MinionRunner::Render()
{
	base::Render();
	return true;
}

void MinionRunner::UpdateMove()
{
	if(!_isAlive) return;


	if(!_hasTargetDir && _player)
	{
		UpdateRotation();    
		_hasTargetDir = true;
	}

	VECTOR vMove = VScale(_vDir, _mvSpeed);
	_vPos		 = VAdd(_vPos, vMove);

	_status = STATUS::WALK;

	if(VSize(_vPos) > 1000.0f)
	{
		_isAlive = false;
	}
}