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
	};

	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

	virtual void UpdateRotation();
	virtual bool Damage(float damage) override;
	void SetPlayer(Player* player) { _player = player; }

protected:

	// ボスの突進処理
	void UpdateRushAttackAI();
	void UpdateIdle(const VECTOR& playerPos);     // 待機状態
	void UpdateRushPrep(const VECTOR& playerPos); // 突進予兆
	void UpdateRushAttack();                      // 突進攻撃
	      


	Player* _player = nullptr;

	BossState _bossState = BossState::IDLE;
	float     _stateTimer = 0.0f;
	VECTOR    _targetDir = VGet(0, 0, 0);
};

