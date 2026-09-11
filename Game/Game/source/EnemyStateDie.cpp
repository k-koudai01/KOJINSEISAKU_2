#include "EnemyStateDie.h"
#include "EnemyBase.h"

void EnemyStateDie::Enter(EnemyBase* enemy)
{
	_dieTimer = 0.0f;
	enemy->SetMvSpeed(0.0f);                  
	enemy->SetStatus(CharaBase::STATUS::DIE); 
}

void EnemyStateDie::Update(EnemyBase* enemy, float deltaTime)
{
	_dieTimer += deltaTime;

	if(_dieTimer >= 1.5f)
	{
		// 死亡完了フラグを立ててオブジェクトを削除対象にする
		enemy->SetAlive(false);
	}
}