#pragma once
#include "appframe.h"

class EnemyBase;

/**
 * @brief 敵AIのステート基底クラス
 */
class EnemyState
{
public:
	virtual ~EnemyState() = default;

	/**
	 * @brief ステート開始時（遷移直後）に1回だけ呼ばれる処理
	 * @param enemy 対象となるEnemyオブジェクトのポインタ
	 */
	virtual void Enter(EnemyBase* enemy) {}

	/**
	 * @brief 毎フレーム呼ばれる更新処理
	 * @param enemy 対象となるEnemyオブジェクトのポインタ
	 * @param deltaTime 1フレームあたりの経過時間
	 */
	virtual void Update(EnemyBase* enemy, float deltaTime) = 0;

	/**
	 * @brief ステート終了時1回だけ呼ばれる処理
	 * @param enemy 対象となるEnemyオブジェクトのポインタ
	 */
	virtual void Exit(EnemyBase* enemy) {}

	/** @brief 現在スタン状態かどうかを取得 */
	virtual bool IsStunned() const { return false; }

	/** @brief 現在突進攻撃中かどうかを取得 */
	virtual bool IsRushing() const { return false; }

protected:
	float _timer = 0.0f;
};

