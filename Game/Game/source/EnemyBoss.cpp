#include "EnemyBoss.h"
#include "EnemyStateIdle.h"

bool EnemyBoss::Initialize()
{
	if(!base::Initialize()) { return false; }

	// ボス専用スプライトシートの設定
	SetSpriteSheet(STATUS::IDLE, "res/Enemy/Enemy_Idle.png", 4, 4);
	SetSpriteSheet(STATUS::WALK, "res/Enemy/Enemy_Run.png", 8, 4);
	SetSpriteSheet(STATUS::RUN, "res/Enemy/Enemy_Run.png", 8, 4);
	SetSpriteSheet(STATUS::DAMAGE, "res/Enemy/Enemy_Damage.png", 6, 4);
	SetSpriteSheet(STATUS::DIE, "res/Enemy/Enemy_Die.png", 8, 4);

	SetSpriteAnimTable
	({
		{ STATUS::IDLE,   { 4, 10.0f, true  } },
		{ STATUS::WALK,   { 8, 10.0f, true  } },
		{ STATUS::RUN,    { 8, 17.0f, true  } },
		{ STATUS::DAMAGE, { 6,  8.0f, false } },
		{ STATUS::DIE,    { 8, 10.0f, false } },
		});

	_hp    = 5.0f; // ボス用のHP
	_vPos  = VGet(100.0f, 0.0f, -25.0f);
	_baseY = _vPos.y;

	// ボス専用の最初のステートを割り当て
	ChangeState(new EnemyStateIdle());
	return true;
}

bool EnemyBoss::Process()
{
	base::Process();
	return true;
}

bool EnemyBoss::Render()
{
	base::Render();
	return true;
}
