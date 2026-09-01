#include "EnemyStateSummonMinions.h"
#include "Enemy.h"
#include "EnemyStateIdle.h"

void EnemyStateSummonMinions::Enter(Enemy* enemy)
{
	EnemyState::Enter(enemy);
	_hasSummoned = false;
	enemy->SetMvSpeed(0.0f);
}

void EnemyStateSummonMinions::Update(Enemy* enemy, float deltaTime)
{
	_timer += deltaTime;

	// 雑魚を召喚
	if(_timer >= 0.8f && !_hasSummoned)
	{
		_hasSummoned = true;
		// TODO: 雑魚生成の関数呼び出し
	}

	// 待機ステートへ戻る
	if(_timer >= 1.5f)
	{
		enemy->ChangeState(new EnemyStateIdle());
	}
}