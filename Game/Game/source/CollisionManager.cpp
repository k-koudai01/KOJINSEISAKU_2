#include "CollisionManager.h"

void CollisionManager::CheckPlayerEnemy(Player* player, EnemyBase* enemy)
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

void CollisionManager::CheckPlayerAttack(Player* player, EnemyBase* enemy)
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

void CollisionManager::CheckPlayerBulletEnemy(Bullet* playerBullet, EnemyBase* enemy)
{
	if(!playerBullet || !enemy) return;
	if(!playerBullet->IsActive() || !enemy->IsAlive()) return;

	const VECTOR pPos = playerBullet->GetPos();

	const VECTOR e0 = enemy->GetCapsuleBottom();
	const VECTOR e1 = enemy->GetCapsuleTop();
	const float  r  = playerBullet->GetCollisionRadius() + enemy->GetCollisionRadius();

	const float distSq = CollisionMath::SegmentPointDistSq(e0, e1, pPos);

	if(distSq <= r * r)
	{
		enemy->Damage(playerBullet->GetDamage());
		playerBullet->Destroy();
	}
}

void CollisionManager::CheckPlayerAttackBullet(Player* player, EnemyReflectBullet* bullet)
{
	if(!player || !bullet) return;
	if(!player->IsAlive() || !bullet->IsActive()) return;
	if(!player->IsAttacking() || bullet->IsReflected()) return;

	const VECTOR p0 = player->GetAttackCapsuleBottom();
	const VECTOR p1 = player->GetAttackCapsuleTop();
	const VECTOR bPos = bullet->GetPos();
	const float  r = player->GetAttackRadius() + bullet->GetCollisionRadius();

	const float distSq = CollisionMath::SegmentPointDistSq(p0, p1, bPos);
	if(distSq <= r * r)
	{
		// 近接攻撃の威力を与える
		bullet->Damage(player->GetDamage());
		if(bullet->GetHP() <= 0.0f)
		{
			bullet->Reflect(player->GetDir());
		}
		// CameraManager::GetInstance()->Shake(30.0f, 0.3f);
	}
}

void CollisionManager::CheckPlayerReflectBullet(Player* player, EnemyReflectBullet* bullet)
{
	if(!player || !bullet) return;
	if(!player->IsAlive() || !bullet->IsActive()) return;

	// 無敵時間中、または【打ち返し済み】の弾ならプレイヤーには当たらない
	if(player->IsInvincible()) return;

	if(bullet->IsReflected()) return;

	const VECTOR pPos = player->GetPos();
	const VECTOR bPos = bullet->GetPos();
	const float  r    = player->GetCollisionRadius() + bullet->GetCollisionRadius();

	// 距離の計算
	float distSq = CollisionMath::PointPointDistSq(pPos, bPos);

	if(distSq <= r * r)
	{
		player->Damage(bullet->GetDamage());
		bullet->Destroy();
	}
}

void CollisionManager::CheckPlayerBulletWithReflectBullet(Player* player, Bullet* playerBullet, EnemyReflectBullet* reflectBullet)
{
	if(!player || !playerBullet || !reflectBullet) return;
	if(!playerBullet->IsActive() || !reflectBullet->IsActive()) return;
	if(reflectBullet->IsReflected()) return;

	const VECTOR pPos = playerBullet->GetPos();
	const VECTOR rPos = reflectBullet->GetPos();
	const float  r = playerBullet->GetCollisionRadius() + reflectBullet->GetCollisionRadius();

	// 距離判定（XZ平面）
	VECTOR pPosXZ = VGet(pPos.x, 0.0f, pPos.z);
	VECTOR rPosXZ = VGet(rPos.x, 0.0f, rPos.z);
	const float distSq = CollisionMath::PointPointDistSq(pPosXZ, rPosXZ);

	if(distSq <= r * r)
	{
		reflectBullet->Damage(playerBullet->GetDamage());

		reflectBullet->OnHitImpact(0.5f, 1.0f);

		if(reflectBullet->GetHP() <= 0.0f)
		{
			reflectBullet->Reflect(playerBullet->GetDir());

			reflectBullet->OnHitImpact(0.1f, 5.0f);
		}

		// プレイヤー弾を消滅
		playerBullet->Destroy();
	}
}

void CollisionManager::CheckBulletEnemy(EnemyReflectBullet* bullet, EnemyBase* enemy)
{
	if(!bullet || !enemy) return;
	if(!bullet->IsActive() || !enemy->IsAlive()) return;
	
	// 打ち返されていない弾はボスにダメージを与えない
	if(!bullet->IsReflected()) return;

	const VECTOR bPos = bullet->GetPos();
	const VECTOR ePos = enemy->GetPos();
	const float  r = bullet->GetCollisionRadius() + enemy->GetCollisionRadius();
	const float distSq = CollisionMath::PointPointDistSq(bPos, ePos);

	if(distSq <= r * r)
	{
		// ボスに弾のダメージを与える
		enemy->Damage(bullet->GetDamage());

		// 着弾したので弾を消去
		bullet->Destroy();
		CameraManager::GetInstance()->Shake(20.0f, 0.2f);
	}
	
}

void CollisionManager::DebugRenderCapsule(const Player* player, const EnemyBase* enemy) const
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

