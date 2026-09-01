#pragma once
#include "Bullet.h"

class Player;
class EnemyReflectBullet : public Bullet
{
	typedef Bullet base;
public:
	EnemyReflectBullet()		  = default;
	virtual ~EnemyReflectBullet() = default;

	/**
	 * @brief 基礎関数
	 */
	bool Initialize(const VECTOR& pos, const VECTOR& dir);
	virtual bool Process() override;
	virtual bool Render() override;

	/**
	 * @brief ダメージ処理
	 * @param damage プレイヤーから受けた攻撃力
	 */
	virtual bool Damage(float damage) override;

	/**
	 * @brief 跳ね返し（軌道変更）処理
	 * @param reflectDir 跳ね返す新しい進行方向ベクトル
	 */
	void Reflect(const VECTOR& reflectDir);

	bool IsReflected() const { return _isReflected; }

private:
	bool _isReflected   = false; // false: 敵の攻撃 / true: プレイヤーの攻撃
};

