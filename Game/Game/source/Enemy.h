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

	virtual bool Damage(float damage) override;
	void SetPlayer(Player* player) { _player = player; }
	

	// スタン状態フラグ
	bool IsStunned() const { return _bossState == BossState::STUN; }
protected:

	// ボスの突進処理
	void UpdateAI();
	void UpdateIdle(const VECTOR& playerPos);     // 待機状態
	void UpdateRushPrep(const VECTOR& playerPos); // 突進予兆
	void UpdateRushAttack();                      // 突進攻撃
	void UpdateStun();                            // スタン状態      

	Player* _player = nullptr;

	BossState _bossState = BossState::IDLE;
	float     _stateTimer = 0.0f;
	VECTOR    _targetDir = VGet(0, 0, 0);
};

