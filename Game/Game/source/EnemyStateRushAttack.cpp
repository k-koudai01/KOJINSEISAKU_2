#include "EnemyStateRushAttack.h"
#include "EnemyBase.h"
#include "EnemyStateStun.h"
#include "EnemyStateIdle.h"

void EnemyStateRushAttack::Enter(EnemyBase* enemy)
{
	EnemyState::Enter(enemy); 
	enemy->SetMvSpeed(8.0f);
}

void EnemyStateRushAttack::Update(EnemyBase* enemy, float deltaTime)
{
	_timer += deltaTime;

	// 前進する
	VECTOR pos = enemy->GetPos();
	VECTOR dir = enemy->GetTargetDir();
	dir.z = 0.0f;

	pos = VAdd(pos, VScale(dir, enemy->GetMvSpeed()));
	enemy->SetPos(pos);

	// 進行方向に向く
	enemy->UpdateFacing(dir);

	// 経過したら突進終了
	if(_timer >= 1.0f)
	{
		// 着地高度を確実に維持
		pos.y = enemy->GetBaseY();
		enemy->SetPos(pos);

		if(enemy->IsParried())
		{
			enemy->ChangeState(new EnemyStateStun()); // パリィされたらスタンへ
		}
		else
		{
			enemy->ChangeState(new EnemyStateIdle()); // 通常は待機へ
		}
	}
}