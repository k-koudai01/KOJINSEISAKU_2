#include "MinionChaser.h"

bool MinionChaser::Initialize()
{
	if (!base::Initialize()) { return false; }

	SetSpriteSheet(STATUS::IDLE, "res/Enemy/Enemy_Idle.png", 4, 4);
	SetSpriteSheet(STATUS::WALK, "res/Enemy/Enemy_Run.png" , 8, 4);

	SetSpriteAnimTable
	({
		{ STATUS::IDLE, { 4, 10.0f, true } },
		{ STATUS::WALK, { 8, 10.0f, true } },
	});

	_spriteScale = 100.0f; 
	_hp			 = 3.0f;    
	_mvSpeed	 = 2.5f;

	_fColSubY		  = 10.0f;
	_fCollisionR	  = 20.0f;
	_fCollisionWeight = 5.0f;

	_status = STATUS::WALK;

	return true;
}

bool MinionChaser::Process()
{
	base::Process();
	UpdateMove();
	return true;
}

bool MinionChaser::Render()
{
	base::Render();
	return true;
}

void MinionChaser::UpdateMove()
{
	// プレイヤーへ向かって移動するロジック
	if(_player && _isAlive)
	{
		UpdateRotation();

		VECTOR vMove = VScale(_vDir, _mvSpeed);
		_vPos        = VAdd(_vPos, vMove);

		_status = STATUS::WALK;
	}
}