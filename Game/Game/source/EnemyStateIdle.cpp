#include "EnemyStateIdle.h"
#include "Enemy.h"
#include "EnemyStateJumpPrep.h"
#include "EnemyStateReflectShoot.h"
#include "EnemyStateSummonMinions.h"

void EnemyStateIdle::Enter(Enemy* enemy)
{
	_timer = 0.0f;
	enemy->SetMvSpeed(0.0f);

	// Y座標を地面の高さに戻す
	VECTOR pos = enemy->GetPos();
	pos.y      = enemy->GetBaseY();
	enemy->SetPos(pos);
}

void EnemyStateIdle::Update(Enemy* enemy, float deltaTime)
{
	_timer += deltaTime;
	enemy->UpdateRotation(); // プレイヤーの方を向く

	// 指定時間が経過したら次の行動をランダム選択
	if(_timer >= 1.5f)
	{
		int nextAction = GetRand(2);

		switch(nextAction)
		{
		case 0:
			enemy->ChangeState(new EnemyStateReflectShoot());
			break;
		case 1:
			enemy->ChangeState(new EnemyStateSummonMinions());
			break;
		case 2:
			enemy->ChangeState(new EnemyStateJumpPrep());
			break;
		}
	}
}