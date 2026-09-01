#include "EnemyStateReflectShoot.h"
#include "Enemy.h"
#include "EnemyStateIdle.h"

void EnemyStateReflectShoot::Enter(Enemy* enemy)
{
	EnemyState::Enter(enemy);
	_hasFired = false;
	enemy->SetMvSpeed(0.0f);
}

void EnemyStateReflectShoot::Update(Enemy* enemy, float deltaTime)
{
	_timer += deltaTime;
	enemy->UpdateRotation(); 

	// 弾を発射
	if(_timer >= 0.5f && !_hasFired)
	{
		_hasFired = true;
		// TODO: ラリー弾生成の関数呼び出し
	}

	// 待機へ戻る
	if(_timer >= 1.2f)
	{
		enemy->ChangeState(new EnemyStateIdle());
	}
}