#include "EnemyStateReflectShoot.h"
#include "Enemy.h"
#include "EnemyStateIdle.h"
#include "BulletManager.h"     
#include "EnemyReflectBullet.h"

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
		
		// 敵の位置と向きを取得
		VECTOR spawnPos = enemy->GetPos();
		VECTOR shootDir = enemy->GetDir();

		// 発射位置を敵の少し前方に調整（敵本体と弾がめり込まないようにする）
		spawnPos = VAdd(spawnPos, VScale(shootDir, 30.0f));

		// ラリー弾を生成・発射
		BulletManager::GetInstance()->Spawn<EnemyReflectBullet>(spawnPos, shootDir);
	}

	// 待機へ戻る
	if(_timer >= 1.2f)
	{
		enemy->ChangeState(new EnemyStateIdle());
	}
}