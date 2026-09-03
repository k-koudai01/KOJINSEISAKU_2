#pragma once
#include "appframe.h"
#include "SpriteCharaBase.h"
#include "EnemyState.h"

class Player;

/**
 * @brief エネミーの共通基底クラス
 * @details 各種パラメータの保持とEnemyStateの管理・共通描画/被弾処理を担当します。
 */
class EnemyBase : public SpriteCharaBase
{
	typedef SpriteCharaBase base;
public:
	EnemyBase() = default;
	virtual ~EnemyBase() = default;

	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

	virtual void UpdateSpriteAnimation(STATUS oldStatus) override;

	// @brief 方向更新処理
	virtual void UpdateRotation();

	void SetPlayer(Player* player) { _player = player; }
	Player* GetPlayer() const { return _player; }

	// メイン関数
	virtual bool Damage(float damage) override;
	void SetParried(bool parried) { _isParried = parried; }

	void ChangeState(EnemyState* newState);

	float GetBaseY() const { return _baseY; }
	void  SetBaseY(float baseY) { _baseY = baseY; }

	VECTOR GetTargetDir() const { return _targetDir; }
	void  SetTargetDir(const VECTOR& dir) { _targetDir = dir; }

	bool IsParried() const { return _isParried; }
	void SetParriedFlag(bool parried) { _isParried = parried; }

	/** @brief StateクラスからSpriteCharaBaseの向き更新を呼ぶための公開用関数 */
	void UpdateFacingPublic(const VECTOR& dir) { UpdateFacing(dir); }

	bool IsStunned() const;
	bool IsRushing() const;

protected:
	Player* _player = nullptr;

	// エネミーの共通状態
	VECTOR _targetDir = VGet(0, 0, 0);
	bool   _isParried = false;
	float  _damageTimer = 0.0f;
	float  _baseY = 0.0f;

	EnemyState* _currentState = nullptr;

private:
	void TerminateState();
	void DebugRender();
};
