#include "EnemyStateJumpPrep.h"
#include "EnemyBase.h"
#include "EnemyStateRushAttack.h"

void EnemyStateJumpPrep::Enter(EnemyBase* enemy)
{
	_timer = 0.0f;
	enemy->SetMvSpeed(0.0f);
}

void EnemyStateJumpPrep::Update(EnemyBase* enemy, float deltaTime)
{
	_timer += deltaTime;
	enemy->UpdateRotation(); 

	float jumpTime = 0.3f; // 1回のジャンプにかかる時間

	// 割った余りを計算し、正規化する
	float progress = fmodf(_timer, jumpTime) / jumpTime; // fmodf()は浮動小数点数の余りを計算する関数

	// サイン波を使ってY軸方向に跳ねさせる
	VECTOR pos = enemy->GetPos();
	pos.y = enemy->GetBaseY() + sinf(progress * 3.14159f) * 30.0f;
	enemy->SetPos(pos);

	if(_timer >= jumpTime * 3.0f)
	{
		// 着地高度を元に戻す
		pos.y = enemy->GetBaseY(); 
		enemy->SetPos(pos);

		// 突進方向とパリィフラグの初期化
		enemy->SetTargetDir(enemy->GetDir());
		enemy->SetParried(false);

		// 突進ステートへ遷移
		enemy->ChangeState(new EnemyStateRushAttack());
	}
}