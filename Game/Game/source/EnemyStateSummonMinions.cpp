#include "EnemyStateSummonMinions.h"
#include "EnemyBase.h"
#include "EnemyBoss.h"
#include "EnemyStateIdle.h"

void EnemyStateSummonMinions::Enter(EnemyBase* enemy)
{
	EnemyState::Enter(enemy);
	_hasSummoned = false;
	enemy->SetMvSpeed(0.0f);
}

void EnemyStateSummonMinions::Update(EnemyBase* enemy, float deltaTime)
{
	_timer += deltaTime;

	// 雑魚を召喚
	if(_timer >= 0.8f && !_hasSummoned)
	{
		_hasSummoned = true;

		if(auto boss = dynamic_cast<EnemyBoss*>(enemy))
		{
			boss->SpawnMinion();
		}
	}

	// 待機ステートへ戻る
	if(_timer >= 1.5f)
	{
		enemy->ChangeState(new EnemyStateIdle());
	}
}