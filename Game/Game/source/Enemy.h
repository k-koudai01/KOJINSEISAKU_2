#pragma once
#include "appframe.h"
#include "SpriteCharaBase.h"

class Player;

class Enemy : public SpriteCharaBase
{
	typedef SpriteCharaBase base;
public:

	// ボスの行動状態
	enum class BossState
	{
		IDLE,
		SHOOT_ATTACK,
		RUSH_PREP,
		RUSH_ATTACK,
		STUN,
	};

	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

	virtual void UpdateSpriteAnimation(STATUS oldStatus) override;
	virtual void UpdateRotation();

	void SetPlayer(Player* player) { _player = player; }

	// 行動関連
	virtual bool Damage(float damage) override;
	void SetParried(bool parried) { _isParried = parried; }

	//　状態関連
	bool IsStunned() const { return _bossState == BossState::STUN;        }
	bool IsRushing() const { return _bossState == BossState::RUSH_ATTACK; }

protected:

	// AI関連
	void UpdateAI();
	void UpdateStatusAndAI();                        // ステータスとAIの更新
	void UpdateIdle(const VECTOR& playerPos);        // 待機状態
	void UpdateShootAttack(const VECTOR& playerPos); // 遠距離攻撃
	void UpdateRushPrep(const VECTOR& playerPos);    // 突進予兆
	void UpdateRushAttack();                         // 突進攻撃
	void UpdateStun();                               // スタン状態      

	// 状態変更
	void ChangeBossState(BossState newState);

	Player* _player = nullptr;

	// ボスの状態
	BossState _bossState  = BossState::IDLE;
	float     _stateTimer = 0.0f;
	bool      _hasFired   = false;
	VECTOR    _targetDir  = VGet(0, 0, 0);
	bool      _isParried  = false;

	float _damageTimer = 0.0f;
private:
	void DebugRender();
};

