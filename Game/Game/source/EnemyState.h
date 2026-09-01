#pragma once
#include "appframe.h"

class Enemy;

class EnemyState
{
public:
	virtual ~EnemyState() = default;

	
    /**
    * @brief ステート開始時（生成・遷移時）に1回だけ呼ばれる処理
    */
	virtual void Enter(Enemy* enemy) {}

	/**
	 * @brief 毎フレーム呼ばれる更新処理
	 * @param enemy 対象の敵インスタンス
	 * @param deltaTime 1フレームあたりの経過時間（通常は 1.0f / 60.0f）
	 */
	virtual void Update(Enemy* enemy, float deltaTime) = 0;


	/**
	 * @brief ステート終了時に1回だけ呼ばれる処理
	 * @param enemy 対象の敵インスタンス
	 */
	virtual void Exit(Enemy* enemy) {}

protected:
	float _timer = 0.0f;
	
};

