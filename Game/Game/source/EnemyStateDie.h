#pragma once
#include "EnemyState.h"

/**
 * @brief 敵AI：死亡ステート
 * @details 死亡アニメーションを再生し、一定時間経過後にオブジェクトを非生存状態にします。
 */
class EnemyStateDie : public EnemyState
{
public:
	virtual void Enter(EnemyBase* enemy) override;
	virtual void Update(EnemyBase* enemy, float deltaTime) override;

private:
	float _dieTimer = 0.0f;
};