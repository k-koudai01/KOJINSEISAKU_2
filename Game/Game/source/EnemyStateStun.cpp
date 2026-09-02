#include "EnemyStateStun.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"

void EnemyStateStun::Enter(EnemyBase* enemy)
{
	EnemyState::Enter(enemy); 
	enemy->SetMvSpeed(0.0f);  

	// 地面に戻る
	VECTOR pos = enemy->GetPos();
	pos.y = enemy->GetBaseY();
	enemy->SetPos(pos);
}

void EnemyStateStun::Update(EnemyBase* enemy, float deltaTime)
{
	_timer += deltaTime;

	// スタン状態を維持し、解除されたら待機ステートへ戻る
	if(_timer >= 2.0f)
	{
		enemy->SetParried(false); // パリィフラグのリセット
		enemy->ChangeState(new EnemyStateIdle());
	}
}