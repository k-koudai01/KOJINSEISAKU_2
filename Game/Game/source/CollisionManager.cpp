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
		if(enemy->IsRushing() && player->IsParryWindow())
		{
			// プレイヤーにダメージが起こらない

			enemy->SetParried(true);

			CameraManager::GetInstance()->Shake(80.0f, 1.0f);

			// ヒットストップ(検討中)
			WaitTimer(100);

			return;
		}

		// ボスがスタン中（STUN）ならダメージは喰らわない
		if(enemy->IsStunned()) return;

		// パリィ失敗でダメージを受ける
		player->Damage(1.0f);
		
	}
}

void CollisionManager::CheckPlayerAttack(Player* player, Enemy* enemy)
{
	if(!player            || !enemy           ) return;
	if(!player->IsAlive() || !enemy->IsAlive()) return;
	if(!enemy->IsStunned()					  ) return;
	if(!player->IsAttackActive()			  ) return;
	if(player->HasHitEnemy()				  ) return;

	const VECTOR p0 = player->GetAttackCapsuleBottom();
	const VECTOR p1 = player->GetAttackCapsuleTop();
	const VECTOR e0 = enemy->GetCapsuleBottom();
	const VECTOR e1 = enemy->GetCapsuleTop();

	const float r = player->GetAttackRadius() + enemy->GetCollisionRadius();
	const float distSq = CollisionMath::SegmentSegmentDistSq(p0, p1, e0, e1);

	if(distSq <= r * r)
	{
		enemy->Damage(1.0f);

		player->SetHasHitEnemy(true);

		CameraManager::GetInstance()->Shake(20.0f, 0.5f);
	}
}

void CollisionManager::CheckCharacterCube(CharaBase* character, Cube* cube)
{
	if(!character || !cube  ) return;
	if(!character->IsAlive()) return;

	VECTOR p0 = character->GetCapsuleBottom();
	VECTOR p1 = character->GetCapsuleTop();
	float r = character->GetCollisionRadius();

	VECTOR boxMin = cube->GetBBMin();
	VECTOR boxMax = cube->GetBBMax();

	// CollisionMathを使って、カプセルと壁の最短距離を調べる
	VECTOR segPos, boxPos;
	float distSq = CollisionMath::SegmentAABBDistSq(p0, p1, boxMin, boxMax, &segPos, &boxPos);

	// 最短距離がキャラクターの半径より小さければ「当たっている」
	if(distSq < r * r)
	{
		float dist = sqrtf(distSq);
		float overlap = r - dist;   // めり込んでいる距離

		// 押し戻す方向
		VECTOR pushDir = VSub(segPos, boxPos);

		// 正規化と安全対策
		if(VSize(pushDir) < 0.0001f)
		{
			pushDir = VGet(0.0f, 0.0f, 1.0f); // とりあえず手前に押し出す
		}
		else
		{
			pushDir = VNorm(pushDir); 
		}

	    // キャラクターをめり込んだ分だけ押し戻す
		VECTOR currentPos = character->GetPos(); 
		VECTOR newPos = VAdd(currentPos, VScale(pushDir, overlap));

		// 新しい位置を更新
		character->SetPos(newPos); 
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

	if(player->IsAttackActive())
	{
		DrawCapsule3D
		(
			player->GetAttackCapsuleBottom(),
			player->GetAttackCapsuleTop(),
			player->GetAttackRadius(),
			divNum,
			GetColor(255, 255, 0),
			spcCol,
			TRUE
		);
	}

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

