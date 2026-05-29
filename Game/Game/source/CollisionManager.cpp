#include "CollisionManager.h"

void CollisionManager::CheckPlayerEnemy(Player* player, Enemy* enemy)
{
	if(!player || !enemy) return;
	if(!player->IsAlive() || !enemy->IsAlive()) { return; }

	// カプセル同士の距離を計算
	const VECTOR p0 = player->GetCapsuleBottom();
	const VECTOR p1 = player->GetCapsuleTop();
	const VECTOR e0 = enemy->GetCapsuleBottom();
	const VECTOR e1 = enemy->GetCapsuleTop();

	// プレイヤーと敵の衝突半径の合計
	const float r = player->GetCollisionRadius() + enemy->GetCollisionRadius();
	const float distSq = CollisionMath::SegmentSegmentDistSq(p0, p1, e0, e1);

	// 当たった
	if (distSq <= r * r)
	{
		player->Damage(1.0f);
	}
}

void CollisionManager::DebugRenderCapsule(const Player* player, const Enemy* enemy) const
{
	if (!player || !enemy) return;
	const int divNum = 8;
	const int spcCol = GetColor(255, 255, 255); 

	DrawCapsule3D
	(
		player->GetCapsuleBottom(),
		player->GetCapsuleTop(),
		player->GetCollisionRadius(),
		divNum,
		GetColor(255, 0, 0),
		spcCol,
		TRUE
	);

	DrawCapsule3D
	(
		enemy->GetCapsuleBottom(),
		enemy->GetCapsuleTop(),
		enemy->GetCollisionRadius(),
		divNum,
		GetColor(0, 0, 255),
		spcCol,
		TRUE
	);
}
