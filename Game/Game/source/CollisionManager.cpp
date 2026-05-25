#include "CollisionManager.h"

void CollisionManager::CheckPlayerEnemy(Player* player, Enemy* enemy)
{
	if(!player || !enemy) return;
	if(player->IsAlive() || enemy->IsAlive()) { return; }

	const VECTOR p = player->GetCollisionCenter();
	const VECTOR e = enemy->GetCollisionCenter();

	const float r = player->GetCollisionRadius() + enemy->GetCollisionRadius();

	const float dx = p.x - e.x;
	const float dy = p.y - e.y;
	const float dz = p.z - e.z;

	const float distSq = dx * dx + dy * dy + dz * dz;

	if (distSq <= r * r)
	{
		player->Damage(1.0f);
	}
}
