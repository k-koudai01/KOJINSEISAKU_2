#pragma once
#include "appframe.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EnemyReflectBullet.h"
#include "Cube.h"
#include "Camera.h"
#include "CameraManager.h"


class CollisionManager
{
public:
	// 更新処理
	void CheckPlayerEnemy(Player* player, EnemyBase* enemy);
	void CheckPlayerAttack(Player* player, EnemyBase* enemy);
	void CheckCharacterCube(CharaBase* character, Cube* cube);

	/** @brief プレイヤーの遠距離弾とボスの衝突・ダメージ判定 */
	void CheckPlayerBulletEnemy(Bullet* playerBullet, EnemyBase* enemy);

	/** @brief プレイヤーの近接攻撃によるラリー弾の一撃打ち返し判定 */
	void CheckPlayerAttackBullet(Player* player, EnemyReflectBullet* bullet);

	/** @brief 敵のラリー弾とプレイヤーの衝突・ダメージ判定 */
	void CheckPlayerReflectBullet(Player* player, EnemyReflectBullet* bullet);

	/** @brief プレイヤーの遠距離弾によるラリー弾の押し返し判定 */
	void CheckPlayerBulletWithReflectBullet(Player* player, Bullet* playerBullet, EnemyReflectBullet* reflectBullet);

	/** @brief 打ち返されたラリー弾とボスの衝突・ダメージ判定 */
	void CheckBulletEnemy(EnemyReflectBullet* bullet, EnemyBase* enemy);

	// 描画処理
	void DebugRenderCapsule(const Player* player, const EnemyBase* enemy) const;
	
protected:
	// カメラ
	Camera* _cam;
};

